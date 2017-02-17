#include "IfStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// IfStmnt
//-----------------------------------------------------------------------------
bool IfStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kIf || tokenType == TokenType::kUnless;
}

IfStmnt * IfStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'if' or 'unless' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("If statement expected!");
        return nullptr;
    }
    
    // Skip the 'if' or 'unless' token and save location.
    // Consume the 'if' or 'unless' token and skip any newlines that follow:
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the if condition and skip any newlines that follow:
    AssignExpr * ifExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(ifExpr, nullptr);
    parseCtx.skipNewlines();
    
    // See if there is a 'then' following. This keyword is optional, unless the 'then' scope is required
    // to be on the same line as the enclosing if statement:
    bool thenScopeRequiresNL = true;
    
    if (parseCtx.tok()->type == TokenType::kThen) {
        // Found a 'then' token: the 'then' scope is allowed to be on the same line.
        // Consume the 'then' token and skip any newlines that follow.
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        thenScopeRequiresNL = false;
    }
    
    // Expect scope following. Parse it and skip any newlines that follow:
    Scope * thenScope = Scope::parse(parseCtx);
    WC_GUARD(thenScope, nullptr);
    parseCtx.skipNewlines();
    
    // See if it violates newline rules.
    // Note: we'll only emit an error in this case and will continue to parse to see
    // if we can find anymore problems.
    if (thenScopeRequiresNL) {
        if (thenScope->getStartToken().startLine == ifExpr->getEndToken().endLine) {
            parseCtx.error(thenScope->getStartToken(),
                           "Code following 'if' statement condition must be on a new line unless "
                           "'then' is used after the condition.");
        }
    }
    
    // 2 possibilities here:
    //
    // 1: 'end' to end the current if block
    // 2: 'else' for an 'else' or 'else if' block.
    if (parseCtx.tok()->type == TokenType::kElse) {
        // 2: 'else' or 'else if' block: consume the 'else' token.
        parseCtx.nextTok();
        
        // See if there is another chained 'else if' or 'else unless'
        if (IfStmnt::peek(parseCtx.tok())) {
            // 'else if' block: parse the if statement following the 'else':
            IfStmnt * outerIfStmnt = IfStmnt::parse(parseCtx);
            WC_GUARD(outerIfStmnt, nullptr);
            
            // Done, return the parsed statement:
            return WC_NEW_AST_NODE(parseCtx,
                                   IfStmntElseIf,
                                   *ifExpr,
                                   *thenScope,
                                   *outerIfStmnt,
                                   *startToken);
        }
        else {
            // 'else' block: parse the scope for the 'else' block and skip any
            // newlines that follow after it:
            Scope * elseScope = Scope::parse(parseCtx);
            WC_GUARD(elseScope, nullptr);
            parseCtx.skipNewlines();
            
            // Else block should be terminated by an 'end' token:
            if (parseCtx.tok()->type != TokenType::kEnd) {
                parseCtx.error("'end' expected to terminate 'else' block!");
                return nullptr;
            }
            
            // Consume the 'end' token and save location
            const Token * endToken = parseCtx.tok();
            parseCtx.nextTok();
            
            // Done, return the parsed statement:
            return WC_NEW_AST_NODE(parseCtx,
                                   IfStmntElse,
                                   *ifExpr,
                                   *thenScope,
                                   *elseScope,
                                   *startToken,
                                   *endToken);
        }
    }
    else {
        // 1: 'if then' type statement: expect closing 'end'
        if (parseCtx.tok()->type != TokenType::kEnd) {
            parseCtx.error("'end' expected to terminate 'if' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Done, return the parsed statement
        return WC_NEW_AST_NODE(parseCtx,
                               IfStmntNoElse,
                               *ifExpr,
                               *thenScope,
                               *startToken,
                               *endToken);
    }
}

IfStmnt::IfStmnt(AssignExpr & ifExpr,
                 Scope & thenScope,
                 const Token & startToken)
:
    mIfExpr(ifExpr),
    mThenScope(thenScope),
    mStartToken(startToken)
{
    mIfExpr.mParent = this;
    mThenScope.mParent = this;
}

const Token & IfStmnt::getStartToken() const {
    return mStartToken;
}

bool IfStmnt::isIfExprInversed() const {
    return mStartToken.type == TokenType::kUnless;
}

//-----------------------------------------------------------------------------
// IfStmntNoElse
//-----------------------------------------------------------------------------
IfStmntNoElse::IfStmntNoElse(AssignExpr & ifExpr,
                             Scope & thenScope,
                             const Token & startToken,
                             const Token & endToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void IfStmntNoElse::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & IfStmntNoElse::getEndToken() const {
    return mEndToken;
}

bool IfStmntNoElse::allCodepathsHaveUncondRet() const {
    return false;
}

//-----------------------------------------------------------------------------
// IfStmntElse
//-----------------------------------------------------------------------------
IfStmntElse::IfStmntElse(AssignExpr & ifExpr,
                         Scope & thenScope,
                         Scope & elseScope,
                         const Token & startToken,
                         const Token & endToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mElseScope(elseScope),
    mEndToken(endToken)
{
    mElseScope.mParent = this;
}

void IfStmntElse::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & IfStmntElse::getEndToken() const {
    return mEndToken;
}

bool IfStmntElse::allCodepathsHaveUncondRet() const {
    return  mThenScope.allCodepathsHaveUncondRet() &&
            mElseScope.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// IfStmntElseIf
//-----------------------------------------------------------------------------
IfStmntElseIf::IfStmntElseIf(AssignExpr & ifExpr,
                             Scope & thenScope,
                             IfStmnt & outerIfStmnt,
                             const Token & startToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mElseIfStmnt(outerIfStmnt)
{
    mElseIfStmnt.mParent = this;
}

void IfStmntElseIf::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & IfStmntElseIf::getEndToken() const {
    return mElseIfStmnt.getEndToken();
}

bool IfStmntElseIf::allCodepathsHaveUncondRet() const {
    return  mThenScope.allCodepathsHaveUncondRet() &&
            mElseIfStmnt.allCodepathsHaveUncondRet();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
