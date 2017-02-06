#include "ShiftExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "UnaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// ShiftExpr
//-----------------------------------------------------------------------------
bool ShiftExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

ShiftExpr * ShiftExpr::parse(ParseCtx & parseCtx) {
    UnaryExpr * leftExpr = UnaryExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            ShiftExpr * rightExpr = ShiftExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.tok()->type) {
        PARSE_OP(TokenType::kLShift, ShiftExprLShift)       // <<
        PARSE_OP(TokenType::kARShift, ShiftExprARShift)     // >>
        PARSE_OP(TokenType::kLRShift, ShiftExprLRShift)     // >>>
            
        default:
            break;
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, ShiftExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// ShiftExprNoOp
//-----------------------------------------------------------------------------
ShiftExprNoOp::ShiftExprNoOp(UnaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void ShiftExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ShiftExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ShiftExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool ShiftExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

bool ShiftExprNoOp::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & ShiftExprNoOp::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// ShiftExprTwoOps
//-----------------------------------------------------------------------------
ShiftExprTwoOps::ShiftExprTwoOps(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & ShiftExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & ShiftExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool ShiftExprTwoOps::isLValue() const {
    return false;
}

bool ShiftExprTwoOps::isConstExpr() const {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

const DataType & ShiftExprTwoOps::dataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

//-----------------------------------------------------------------------------
// ShiftExprLShift
//-----------------------------------------------------------------------------
ShiftExprLShift::ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprARShift
//-----------------------------------------------------------------------------
ShiftExprARShift::ShiftExprARShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprARShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprLRShift
//-----------------------------------------------------------------------------
ShiftExprLRShift::ShiftExprLRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLRShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
