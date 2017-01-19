#include "WCMulExpr.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssert.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCShiftExpr.hpp"

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

bool MulExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool MulExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & MulExprNoOp::dataType() {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * MulExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * MulExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * MulExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// MulExprTwoOps
//-----------------------------------------------------------------------------
MulExprTwoOps::MulExprTwoOps(ShiftExpr & leftExpr,
                             MulExpr & rightExpr
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

const Token & MulExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulExprTwoOps::isLValue() {
    return false;
}

bool MulExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & MulExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * MulExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a binary expression result!");
    return nullptr;
}

llvm::Value * MulExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
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

llvm::Constant * MulExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
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
// MulExprMul
//-----------------------------------------------------------------------------
MulExprMul::MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                  , &DataType::codegenMulOp, &DataType::codegenConstMulOp
                #endif
                  )
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
    MulExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                  , &DataType::codegenDivOp, &DataType::codegenConstDivOp
                #endif
                  )
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
    MulExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                  , &DataType::codegenModOp, &DataType::codegenConstModOp
                #endif
                  )
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
    MulExprTwoOps(leftExpr, rightExpr
                #warning FIXME - Codegen
                #if 0
                  , &DataType::codegenBAndOp, &DataType::codegenConstBAndOp
                #endif
                  )
{
    WC_EMPTY_FUNC_BODY();
}

void MulExprBAnd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
