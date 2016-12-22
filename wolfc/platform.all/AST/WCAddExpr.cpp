#include "WCAddExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCMulExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AddExpr
//-----------------------------------------------------------------------------
bool AddExpr::peek(const Token * tokenPtr) {
    return MulExpr::peek(tokenPtr);
}

AddExpr * AddExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    MulExpr * leftExpr = MulExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            ++tokenPtr;\
            AddExpr * rightExpr = AddExpr::parse(tokenPtr, alloc);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(alloc, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (tokenPtr[0].type) {
        PARSE_OP(TokenType::kPlus, AddExprAdd)      // +
        PARSE_OP(TokenType::kMinus, AddExprSub)     // -
        PARSE_OP(TokenType::kVBar, AddExprBOr)      // |
        PARSE_OP(TokenType::kHat, AddExprBXor)      // ^
            
        default:
            break;
    }
    
    #undef PARSE_OP
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, AddExprNoOp, *leftExpr);
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
AddExprTwoOps::AddExprTwoOps(MulExpr & leftExpr, AddExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
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
    compileError("Can't get the address of '+' or '-' operator result!");
    return nullptr;
}

llvm::Value * AddExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Value * leftVal = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Value * rightVal = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Codegen the op itself
    return codegenOpEval(cgCtx, *leftVal, *rightVal);
}

llvm::Constant * AddExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Constant * leftVal = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Constant * rightVal = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Codegen the op itself
    return codegenOpConstEval(*leftVal, *rightVal);
}

bool AddExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// AddExprAdd
//-----------------------------------------------------------------------------
AddExprAdd::AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddExprAdd::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenAddOp(cgCtx, *this, leftVal, rightTy, rightVal);
}

llvm::Constant * AddExprAdd::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenConstAddOp(*this, leftVal, rightTy, rightVal);
}

//-----------------------------------------------------------------------------
// AddExprSub
//-----------------------------------------------------------------------------
AddExprSub::AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddExprSub::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenSubOp(cgCtx, *this, leftVal, rightTy, rightVal);
}

llvm::Constant * AddExprSub::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenConstSubOp(*this, leftVal, rightTy, rightVal);
}

//-----------------------------------------------------------------------------
// AddExprBOr
//-----------------------------------------------------------------------------
AddExprBOr::AddExprBOr(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddExprBOr::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenBOrOp(cgCtx, *this, leftVal, rightTy, rightVal);
}

llvm::Constant * AddExprBOr::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenConstBOrOp(*this, leftVal, rightTy, rightVal);
}

//-----------------------------------------------------------------------------
// AddExprBXor
//-----------------------------------------------------------------------------
AddExprBXor::AddExprBXor(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddExprBXor::codegenOpEval(CodegenCtx & cgCtx,
                                         llvm::Value & leftVal,
                                         llvm::Value & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenBXOrOp(cgCtx, *this, leftVal, rightTy, rightVal);
}

llvm::Constant * AddExprBXor::codegenOpConstEval(llvm::Constant & leftVal,
                                                 llvm::Constant & rightVal)
{
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return leftTy.codegenConstBXOrOp(*this, leftVal, rightTy, rightVal);
}

WC_END_NAMESPACE
