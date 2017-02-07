#include "LOrExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LAndExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LOrExpr
//-----------------------------------------------------------------------------
bool LOrExpr::peek(const Token * tokenPtr) {
    return LAndExpr::peek(tokenPtr);
}

LOrExpr * LOrExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    LAndExpr * andExpr = LAndExpr::parse(parseCtx);
    WC_GUARD(andExpr, nullptr);
    
    // See if there is an 'or' for logical or:
    if (parseCtx.tok()->type == TokenType::kOr) {
        TokenType nextTokType = parseCtx.tok()[1].type;

        // Note: do not parse if we find 'or if' or 'or unless' since those are block
        // terminators for a 'if / or if / else' chain
        if (nextTokType != TokenType::kIf &&
            nextTokType != TokenType::kUnless)
        {
            // Or expression with or. Skip the 'or'
            parseCtx.nextTok();
        
            // Parse the following and expression and create the AST node
            LOrExpr * orExpr = LOrExpr::parse(parseCtx);
            WC_GUARD(orExpr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, LOrExprOr, *andExpr, *orExpr);
        }
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(parseCtx, LOrExprNoOp, *andExpr);
}

//-----------------------------------------------------------------------------
// LOrExprNoOp
//-----------------------------------------------------------------------------
LOrExprNoOp::LOrExprNoOp(LAndExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void LOrExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LOrExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & LOrExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// LOrExprOr
//-----------------------------------------------------------------------------
LOrExprOr::LOrExprOr(LAndExpr & leftExpr, LOrExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

void LOrExprOr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LOrExprOr::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & LOrExprOr::getEndToken() const {
    return mRightExpr.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
