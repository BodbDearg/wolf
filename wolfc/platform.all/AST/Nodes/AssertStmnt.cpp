#include "AssertStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool AssertStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kAssert;
}

AssertStmnt * AssertStmnt::parse(ParseCtx & parseCtx) {
    if (parseCtx.tok()->type != TokenType::kAssert) {
        parseCtx.error("Expected keyword 'assert' for 'assert()' statement!");
        return nullptr;
    }
    
    const Token * assertTok = parseCtx.tok();
    parseCtx.nextTok();         // Consume 'assert'
    parseCtx.skipNewlines();    // Skip any newlines
    
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expected '(' following 'assert'!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Consume '('
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Parse the inner expression
    AssignExpr * assignExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(assignExpr, nullptr);
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Expect ')' following all that:
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expected closing ')' for 'assert()' statement!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // Create and return the print statement
    return WC_NEW_AST_NODE(parseCtx, AssertStmnt, *assertTok, *assignExpr, *closingParenTok);
}

AssertStmnt::AssertStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

void AssertStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & AssertStmnt::getStartToken() const {
    return mStartToken;
}

const Token & AssertStmnt::getEndToken() const {
    return mEndToken;
}

bool AssertStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
