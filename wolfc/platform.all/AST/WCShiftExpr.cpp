#include "WCShiftExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCUnaryExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// ShiftExpr
//-----------------------------------------------------------------------------

bool ShiftExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

ShiftExpr * ShiftExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    UnaryExpr * leftExpr = UnaryExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            ++tokenPtr;\
            ShiftExpr * rightExpr = ShiftExpr::parse(tokenPtr, alloc);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(alloc, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (tokenPtr[0].type) {
        PARSE_OP(TokenType::kLShift, ShiftExprLShift)           // <<
        PARSE_OP(TokenType::kARShift, ShiftExprArithRShift)     // >>
        PARSE_OP(TokenType::kLRShift, ShiftExprLogicRShift)     // >>>
            
        default:
            break;
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, ShiftExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// ShiftExprNoOp
//-----------------------------------------------------------------------------

ShiftExprNoOp::ShiftExprNoOp(UnaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
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

llvm::Value * ShiftExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * ShiftExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * ShiftExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
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

llvm::Value * ShiftExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of '*' or '/' operator result!");
    return nullptr;
}

bool ShiftExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// ShiftExprLShift
//-----------------------------------------------------------------------------

ShiftExprLShift::ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * ShiftExprLShift::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateShl(left, right, "ShiftExprLShift_LShiftOp");
}

llvm::Constant * ShiftExprLShift::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getShl(left, right);
}

//-----------------------------------------------------------------------------
// ShiftExprArithRShift
//-----------------------------------------------------------------------------

ShiftExprArithRShift::ShiftExprArithRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * ShiftExprArithRShift::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateAShr(left, right, "ShiftExprArithRShift_ARShiftOp");
}

llvm::Constant * ShiftExprArithRShift::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getAShr(left, right);
}

//-----------------------------------------------------------------------------
// ShiftExprLogicRShift
//-----------------------------------------------------------------------------

ShiftExprLogicRShift::ShiftExprLogicRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * ShiftExprLogicRShift::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateLShr(left, right, "ShiftExprLogicRShift_LRShiftOp");
}

llvm::Constant * ShiftExprLogicRShift::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getLShr(left, right);
}

WC_END_NAMESPACE
