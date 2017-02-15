#include "WhileStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool WhileStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kWhile || tokenType == TokenType::kUntil;
}

WhileStmnt * WhileStmnt::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // Parse the initial 'while' or 'until' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("While statement expected!");
        return nullptr;
    }
    
    // Skip the 'while' or 'until' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the while expression (while condition):
    AssignExpr * whileExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(whileExpr, nullptr);
    
    // See if there is a 'do' following. This keyword is optional, unless the body scope is required
    // to be on the same line as the enclosing while statement:
    bool bodyScopeRequiresNL = true;
    
    if (parseCtx.tok()->type == TokenType::kDo) {
        // Found a 'do' token, skip it. The body scope is allowed to be on the same line
        parseCtx.nextTok();
        bodyScopeRequiresNL = false;
    }
    
    // Expect scope following:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    
    // See if it violates newline rules:
    if (bodyScopeRequiresNL) {
        if (bodyScope->getStartToken().startLine == whileExpr->getEndToken().endLine) {
            parseCtx.error(bodyScope->getStartToken(),
                           "Code following 'while/until' statement condition must be on a new line unless "
                           "'do' is used after the condition.");
            
            return nullptr;
        }
    }
    
    // While block should be terminated by an 'end' token:
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate 'while/until' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(parseCtx, WhileStmnt, *whileExpr, *bodyScope, *startToken, *endToken);
}

WhileStmnt::WhileStmnt(AssignExpr & whileExpr,
                       Scope & bodyScope,
                       const Token & startToken,
                       const Token & endToken)
:
    mWhileExpr(whileExpr),
    mBodyScope(bodyScope),
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_ASSERT(mStartToken.type == TokenType::kWhile || mStartToken.type == TokenType::kUntil);
    mWhileExpr.mParent = this;
    mBodyScope.mParent = this;
}

void WhileStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & WhileStmnt::getStartToken() const {
    return mStartToken;
}

const Token & WhileStmnt::getEndToken() const {
    return mEndToken;
}

bool WhileStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

bool WhileStmnt::isWhileExprInversed() const {
    return mStartToken.type == TokenType::kUntil;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
