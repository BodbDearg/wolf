#include "WCAddExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCMulExpr.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AddExpr
//-----------------------------------------------------------------------------
bool AddExpr::peek(const Token * tokenPtr) {
    return MulExpr::peek(tokenPtr);
}

AddExpr * AddExpr::parse(ParseCtx & parseCtx) {
    MulExpr * leftExpr = MulExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            AddExpr * rightExpr = AddExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.curTok->type) {
        PARSE_OP(TokenType::kPlus, AddExprAdd)      // +
        PARSE_OP(TokenType::kMinus, AddExprSub)     // -
        PARSE_OP(TokenType::kVBar, AddExprBOr)      // |
        PARSE_OP(TokenType::kHat, AddExprBXor)      // ^
            
        default:
            break;
    }
    
    #undef PARSE_OP
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, AddExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// AddExprNoOp
//-----------------------------------------------------------------------------
AddExprNoOp::AddExprNoOp(MulExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & AddExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AddExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool AddExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool AddExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & AddExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * AddExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AddExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * AddExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// AddExprTwoOps
//-----------------------------------------------------------------------------
AddExprTwoOps::AddExprTwoOps(MulExpr & leftExpr,
                             AddExpr & rightExpr,
                             DTCodegenBinaryOpFunc codegenBinaryOpFunc,
                             DTCodegenConstBinaryOpFunc codegenConstBinaryOpFunc)
:
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr),
    mCodegenBinaryOpFunc(codegenBinaryOpFunc),
    mCodegenConstBinaryOpFunc(codegenConstBinaryOpFunc)
{
    WC_ASSERT(mCodegenBinaryOpFunc);
    WC_ASSERT(mCodegenConstBinaryOpFunc);
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AddExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AddExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AddExprTwoOps::isLValue() {
    return false;
}

bool AddExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & AddExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * AddExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a binary expression result!");
    return nullptr;
}

llvm::Value * AddExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
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

llvm::Constant * AddExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
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

//-----------------------------------------------------------------------------
// AddExprAdd
//-----------------------------------------------------------------------------
AddExprAdd::AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr, &DataType::codegenAddOp, &DataType::codegenConstAddOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AddExprSub
//-----------------------------------------------------------------------------
AddExprSub::AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr, &DataType::codegenSubOp, &DataType::codegenConstSubOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AddExprBOr
//-----------------------------------------------------------------------------
AddExprBOr::AddExprBOr(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr, &DataType::codegenBOrOp, &DataType::codegenConstBOrOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AddExprBXor
//-----------------------------------------------------------------------------
AddExprBXor::AddExprBXor(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr, &DataType::codegenBXOrOp, &DataType::codegenConstBXOrOp)
{
    WC_EMPTY_FUNC_BODY();
}

WC_END_NAMESPACE
