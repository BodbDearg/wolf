#include "LAndExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "NotExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LAndExpr
//-----------------------------------------------------------------------------
bool LAndExpr::peek(const Token * tokenPtr) {
    return NotExpr::peek(tokenPtr);
}

LAndExpr * LAndExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    NotExpr * leftExpr = NotExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is an 'and' for logical and
    if (parseCtx.tok()->type == TokenType::kAnd) {
        // And expression with and. Skip the 'and'
        parseCtx.nextTok();
        
        // Parse the following and expression and create the AST node
        LAndExpr * rightExpr = LAndExpr::parse(parseCtx);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(parseCtx, LAndExprAnd, *leftExpr, *rightExpr);
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(parseCtx, LAndExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// LAndExprNoOp
//-----------------------------------------------------------------------------
LAndExprNoOp::LAndExprNoOp(NotExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void LAndExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LAndExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & LAndExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

const DataType & LAndExprNoOp::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// LAndExprAnd
//-----------------------------------------------------------------------------
LAndExprAnd::LAndExprAnd(NotExpr & leftExpr, LAndExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

void LAndExprAnd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LAndExprAnd::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & LAndExprAnd::getEndToken() const {
    return mRightExpr.getEndToken();
}

const DataType & LAndExprAnd::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
