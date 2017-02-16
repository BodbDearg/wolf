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
    
    // Skip the 'if' or 'unless' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();         // Consume 'if' or 'unless'
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(ifExpr, nullptr);
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // See if there is a 'then' following. This keyword is optional, unless the 'then' scope is required
    // to be on the same line as the enclosing if statement:
    bool thenScopeRequiresNL = true;
    
    if (parseCtx.tok()->type == TokenType::kThen) {
        // Found a 'then' token: the 'then' scope is allowed to be on the same line
        parseCtx.nextTok();             // Consume 'then'
        parseCtx.skipNewlines();        // Skip any newlines that follow
        thenScopeRequiresNL = false;
    }
    
    // Expect scope following:
    Scope * thenScope = Scope::parse(parseCtx);
    WC_GUARD(thenScope, nullptr);
    parseCtx.skipNewlines();            // Skip any newlines that follow
    
    // See if it violates newline rules:
    if (thenScopeRequiresNL) {
        if (thenScope->getStartToken().startLine == ifExpr->getEndToken().endLine) {
            parseCtx.error(thenScope->getStartToken(),
                           "Code following 'if' statement condition must be on a new line unless "
                           "'then' is used after the condition.");
            
            return nullptr;
        }
    }
    
    // 3 possibilities can follow:
    //
    // 1 - 'end', for a simple 'if' statement without any 'or if' or 'else'.
    // 2 - 'or if' for an chained 'elseif' type statement
    // 3 - 'else' for an 'if' statement with an else block
    //
    if (parseCtx.tok()->type == TokenType::kElse) {
        // (3) if statement with an else
        parseCtx.nextTok();         // Consume the 'else' token
        parseCtx.skipNewlines();    // Skip any newlines that follow
        
        // Parse the scope for the 'else' block:
        Scope * elseScope = Scope::parse(parseCtx);
        WC_GUARD(elseScope, nullptr);
        parseCtx.skipNewlines();    // Skip any newlines that follow
        
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
    else if (parseCtx.tok()->type == TokenType::kOr) {
        // (2) if statement with an 'or if' chained if statement
        parseCtx.nextTok();         // Skip the 'or' token
        parseCtx.skipNewlines();    // Skip any newlines that follow
        
        // Parse the if statement following the 'or':
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
        // (1) 'if then' type statement: expect closing 'end'
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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
