#include "WCShiftExpr.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssert.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCUnaryExpr.hpp"

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

void ShiftExprNoOp::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

const Token & ShiftExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ShiftExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool ShiftExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool ShiftExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & ShiftExprNoOp::dataType() {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * ShiftExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * ShiftExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * ShiftExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// ShiftExprTwoOps
//-----------------------------------------------------------------------------
ShiftExprTwoOps::ShiftExprTwoOps(UnaryExpr & leftExpr,
                                 ShiftExpr & rightExpr
                                #warning FIXME - Codegen
                                #if 0
                                 ,DTCodegenBinaryOpFunc codegenBinaryOpFunc,
                                 DTCodegenConstBinaryOpFunc codegenConstBinaryOpFunc
                                #endif
                                 )
:
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
#warning FIXME - Codegen
#if 0
    ,mCodegenBinaryOpFunc(codegenBinaryOpFunc),
    mCodegenConstBinaryOpFunc(codegenConstBinaryOpFunc)
#endif
{
#warning FIXME - Codegen
#if 0
    WC_ASSERT(mCodegenBinaryOpFunc);
    WC_ASSERT(mCodegenConstBinaryOpFunc);
#endif
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & ShiftExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & ShiftExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool ShiftExprTwoOps::isLValue() {
    return false;
}

bool ShiftExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & ShiftExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * ShiftExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a binary expression result!");
    return nullptr;
}

llvm::Value * ShiftExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Value * leftVal = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Value * rightVal = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // Do the operation and return the result
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return (leftTy.*mCodegenBinaryOpFunc)(cgCtx, *this, *leftVal, rightTy, *rightVal);
}

llvm::Constant * ShiftExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Constant * leftVal = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Constant * rightVal = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // Do the operation and return the result
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return (leftTy.*mCodegenConstBinaryOpFunc)(*this, *leftVal, rightTy, *rightVal);
}
#endif

//-----------------------------------------------------------------------------
// ShiftExprLShift
//-----------------------------------------------------------------------------
ShiftExprLShift::ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                    , &DataType::codegenLShiftOp, &DataType::codegenConstLShiftOp
                #endif
                    )
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLShift::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprARShift
//-----------------------------------------------------------------------------
ShiftExprARShift::ShiftExprARShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                    , &DataType::codegenARShiftOp, &DataType::codegenConstARShiftOp
                #endif
                    )
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprARShift::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprLRShift
//-----------------------------------------------------------------------------
ShiftExprLRShift::ShiftExprLRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                    , &DataType::codegenLRShiftOp, &DataType::codegenConstLRShiftOp
                #endif
                    )
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLRShift::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
