#include "WCAssignExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCTernaryExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AssignExpr
//-----------------------------------------------------------------------------
bool AssignExpr::peek(const Token * tokenPtr) {
    return TernaryExpr::peek(tokenPtr);
}

AssignExpr * AssignExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    TernaryExpr * leftExpr = TernaryExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);

    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            AssignExpr * rightExpr = AssignExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.curTok->type) {
        PARSE_OP(TokenType::kAssign, AssignExprAssign)                  // =
        PARSE_OP(TokenType::kAssignBOr, AssignExprAssignBOr)            // |=
        PARSE_OP(TokenType::kAssignBXor, AssignExprAssignBXor)          // ^=
        PARSE_OP(TokenType::kAssignBAnd, AssignExprAssignBAnd)          // &=
        PARSE_OP(TokenType::kAssignAdd, AssignExprAssignAdd)            // +=
        PARSE_OP(TokenType::kAssignSub, AssignExprAssignSub)            // -=
        PARSE_OP(TokenType::kAssignMul, AssignExprAssignMul)            // *=
        PARSE_OP(TokenType::kAssignDiv, AssignExprAssignDiv)            // /=
        PARSE_OP(TokenType::kAssignMod, AssignExprAssignMod)            // %=
        PARSE_OP(TokenType::kAssignLShift, AssignExprAssignLShift)      // <<=
        PARSE_OP(TokenType::kAssignARShift, AssignExprAssignARShift)    // >>=
        PARSE_OP(TokenType::kAssignLRShift, AssignExprAssignLRShift)    // >>>=
            
        default:
            break;
    }
    
    #undef PARSE_OP

    // Assign expression with no assign
    return WC_NEW_AST_NODE(parseCtx, AssignExprNoAssign, *leftExpr);
}

//-----------------------------------------------------------------------------
// AssignExprNoAssign
//-----------------------------------------------------------------------------
AssignExprNoAssign::AssignExprNoAssign(TernaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & AssignExprNoAssign::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AssignExprNoAssign::getEndToken() const {
    return mExpr.getEndToken();
}

bool AssignExprNoAssign::isLValue() {
    return mExpr.isLValue();
}

bool AssignExprNoAssign::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & AssignExprNoAssign::dataType() {
    return mExpr.dataType();
}

llvm::Value * AssignExprNoAssign::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AssignExprNoAssign::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * AssignExprNoAssign::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// AssignExprAssignBase
//-----------------------------------------------------------------------------
AssignExprAssignBase::AssignExprAssignBase(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AssignExprAssignBase::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AssignExprAssignBase::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AssignExprAssignBase::isLValue() {
    return false;
}

bool AssignExprAssignBase::isConstExpr() {
    // TODO: eventually we could relax this perhaps and allow for full constant evaluation of a
    // function provided it does not rely on something that can externally vary.
    // For now however, you must use functional type programming if you want constant evaluation.
    return false;
}

DataType & AssignExprAssignBase::dataType() {
    // TODO: Handle auto type promotion eventually
    return mLeftExpr.dataType();
}

llvm::Value * AssignExprAssignBase::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an assign type ('=') expression result!");
    return nullptr;
}

llvm::Constant * AssignExprAssignBase::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: eventually we could relax this perhaps and allow for full constant evaluation of a
    // function provided it does not rely on something that can externally vary.
    // For now however, you must use functional type programming if you want constant evaluation.
    WC_UNUSED_PARAM(cgCtx);
    compileError("Assign is not allowed in a constant expression!");
    return nullptr;
}

bool AssignExprAssignBase::compileCheckAssignIsLegal() {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        compileError("Can't assign to an rvalue!");
        return false;
    }

    // TODO: support auto type promotion and the lark
    // The data type of the right must match the left
    const DataType & leftDataType = mLeftExpr.dataType();
    const DataType & rightDataType = mRightExpr.dataType();

    // TODO: print variable name here
    if (!leftDataType.equals(rightDataType)) {
        compileError("Can't assign expression of type '%s' to variable of type '%s'!",
            rightDataType.name().c_str(),
            leftDataType.name().c_str());

        return false;
    }

    // All good!
    return true;
}

//-----------------------------------------------------------------------------
// AssignExprBinaryOpBase
//-----------------------------------------------------------------------------
AssignExprBinaryOpBase::AssignExprBinaryOpBase(TernaryExpr & leftExpr,
                                               AssignExpr & rightExpr,
                                               DTCodegenBinaryOpFunc codegenBinaryOpFunc)
:
    AssignExprAssignBase(leftExpr, rightExpr),
    mCodegenBinaryOpFunc(codegenBinaryOpFunc)
{
    WC_ASSERT(mCodegenBinaryOpFunc);
}

llvm::Value * AssignExprBinaryOpBase::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftVal = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftVal);
    llvm::Value * rightVal = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightVal, nullptr);

    // Do the operation and store the result on the left
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    llvm::Value * newVal = (leftTy.*mCodegenBinaryOpFunc)(cgCtx,
                                                          *this,
                                                          *leftVal,
                                                          rightTy,
                                                          *rightVal);
    
    WC_GUARD(newVal, nullptr);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newVal, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newVal;
}

//-----------------------------------------------------------------------------
// AssignExprAssign
//-----------------------------------------------------------------------------
AssignExprAssign::AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssign::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left address and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(rightValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the 
    // right expression, so return that...
    return rightValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignAdd
//-----------------------------------------------------------------------------
AssignExprAssignAdd::AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenAddOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignSub
//-----------------------------------------------------------------------------
AssignExprAssignSub::AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenSubOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignBOr
//-----------------------------------------------------------------------------
AssignExprAssignBOr::AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenBOrOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignBXor
//-----------------------------------------------------------------------------
AssignExprAssignBXor::AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenBXOrOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignMul
//-----------------------------------------------------------------------------
AssignExprAssignMul::AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenMulOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignDiv
//-----------------------------------------------------------------------------
AssignExprAssignDiv::AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenDivOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignMod
//-----------------------------------------------------------------------------
AssignExprAssignMod::AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenModOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignBAnd
//-----------------------------------------------------------------------------
AssignExprAssignBAnd::AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenBAndOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignLShift
//-----------------------------------------------------------------------------
AssignExprAssignLShift::AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenLShiftOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignARShift
//-----------------------------------------------------------------------------
AssignExprAssignARShift::AssignExprAssignARShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenARShiftOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssignLRShift
//-----------------------------------------------------------------------------
AssignExprAssignLRShift::AssignExprAssignLRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr, &DataType::codegenLRShiftOp)
{
    WC_EMPTY_FUNC_BODY();
}

WC_END_NAMESPACE
