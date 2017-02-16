#include "NotExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "CmpExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotExpr
//-----------------------------------------------------------------------------
bool NotExpr::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    
    return  tokenType == TokenType::kNot ||
            tokenType == TokenType::kTilde ||
            CmpExpr::peek(tokenPtr);
}

NotExpr * NotExpr::parse(ParseCtx & parseCtx) {
    // Save the first token:
    const Token * startToken = parseCtx.tok();
    
    // See if an actual 'not' or bitwise not ('~') expression:
    if (startToken->type == TokenType::kNot) {
        // Logical 'not': skip 'not' and any newlines that follow
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprLNot, *notExpr, *startToken);
    }
    else if (startToken->type == TokenType::kTilde) {
        // Bitwise '~' (not): skip '~' and any newlines that follow
        parseCtx.nextTok();
        parseCtx.skipNewlines();

        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);

        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprBNot, *notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    CmpExpr * addSubExpr = CmpExpr::parse(parseCtx);
    WC_GUARD(addSubExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, NotExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// NotExprNoOp
//-----------------------------------------------------------------------------
NotExprNoOp::NotExprNoOp(CmpExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void NotExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// NotExprLNot
//-----------------------------------------------------------------------------
NotExprLNot::NotExprLNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

void NotExprLNot::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprLNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprLNot::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// NotExprBNot
//-----------------------------------------------------------------------------
NotExprBNot::NotExprBNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

void NotExprBNot::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprBNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprBNot::getEndToken() const {
    return mExpr.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
