#include "MulExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "ShiftExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// MulExpr
//-----------------------------------------------------------------------------
bool MulExpr::peek(const Token * tokenPtr) {
    return ShiftExpr::peek(tokenPtr);
}

MulExpr * MulExpr::parse(ParseCtx & parseCtx) {
    ShiftExpr * leftExpr = ShiftExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            MulExpr * rightExpr = MulExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.tok()->type) {
        PARSE_OP(TokenType::kAsterisk, MulExprMul)      // *
        PARSE_OP(TokenType::kSlash, MulExprDiv)         // /
        PARSE_OP(TokenType::kPercent, MulExprMod)       // %
        PARSE_OP(TokenType::kAmpersand, MulExprBAnd)    // &
            
        default:
            break;
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, MulExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// MulExprNoOp
//-----------------------------------------------------------------------------
MulExprNoOp::MulExprNoOp(ShiftExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void MulExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & MulExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & MulExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool MulExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

bool MulExprNoOp::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & MulExprNoOp::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// MulExprTwoOps
//-----------------------------------------------------------------------------
MulExprTwoOps::MulExprTwoOps(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & MulExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulExprTwoOps::isLValue() const {
    return false;
}

bool MulExprTwoOps::isConstExpr() const {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

const DataType & MulExprTwoOps::dataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

//-----------------------------------------------------------------------------
// MulExprMul
//-----------------------------------------------------------------------------
MulExprMul::MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void MulExprMul::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// MulExprDiv
//-----------------------------------------------------------------------------
MulExprDiv::MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void MulExprDiv::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// MulExprMod
//-----------------------------------------------------------------------------
MulExprMod::MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void MulExprMod::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// MulExprBAnd
//-----------------------------------------------------------------------------
MulExprBAnd::MulExprBAnd(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void MulExprBAnd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
