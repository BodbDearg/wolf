#include "WCMulDivExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCShiftExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// MulDivExpr
//-----------------------------------------------------------------------------

bool MulDivExpr::peek(const Token * tokenPtr) {
    return ShiftExpr::peek(tokenPtr);
}

MulDivExpr * MulDivExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    ShiftExpr * leftExpr = ShiftExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if '*' or '/' following:
    if (tokenPtr->type == TokenType::kAsterisk && MulDivExpr::peek(tokenPtr + 1)) {
        // Mul operation: Skip '*'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        MulDivExpr * rightExpr = MulDivExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, MulDivExprMul, *leftExpr, *rightExpr);
    }
    else if (tokenPtr->type == TokenType::kSlash && MulDivExpr::peek(tokenPtr + 1)) {
        // Div operation: Skip '/'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        MulDivExpr * rightExpr = MulDivExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, MulDivExprDiv, *leftExpr, *rightExpr);
    }
    else if (tokenPtr->type == TokenType::kPercent && MulDivExpr::peek(tokenPtr + 1)) {
        // Modulo operation: Skip '%'
        ++tokenPtr;

        // Parse following expr and return combined expr
        MulDivExpr * rightExpr = MulDivExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, MulDivExprMod, *leftExpr, *rightExpr);
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, MulDivExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// MulDivExprNoOp
//-----------------------------------------------------------------------------

MulDivExprNoOp::MulDivExprNoOp(ShiftExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & MulDivExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & MulDivExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool MulDivExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool MulDivExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & MulDivExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * MulDivExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * MulDivExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * MulDivExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// MulDivExprTwoOps
//-----------------------------------------------------------------------------

MulDivExprTwoOps::MulDivExprTwoOps(ShiftExpr & leftExpr, MulDivExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & MulDivExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulDivExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulDivExprTwoOps::isLValue() {
    return false;
}

bool MulDivExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & MulDivExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * MulDivExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of '*' or '/' operator result!");
    return nullptr;
}

bool MulDivExprTwoOps::compileCheckBothExprsAreInt() const {
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
// MulDivExprMul
//-----------------------------------------------------------------------------

MulDivExprMul::MulDivExprMul(ShiftExpr & leftExpr, MulDivExpr & rightExpr) :
    MulDivExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulDivExprMul::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateMul(left, right, "MulDivExprMul_MulOp");
}

llvm::Constant * MulDivExprMul::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getMul(left, right);
}

//-----------------------------------------------------------------------------
// MulDivExprDiv
//-----------------------------------------------------------------------------

MulDivExprDiv::MulDivExprDiv(ShiftExpr & leftExpr, MulDivExpr & rightExpr) :
    MulDivExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulDivExprDiv::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSDiv(left, right, "MulDivExprDiv_DivOp");
}

llvm::Constant * MulDivExprDiv::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getSDiv(left, right);
}

//-----------------------------------------------------------------------------
// MulDivExprMod
//-----------------------------------------------------------------------------

MulDivExprMod::MulDivExprMod(ShiftExpr & leftExpr, MulDivExpr & rightExpr) :
    MulDivExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulDivExprMod::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSRem(left, right, "MulDivExprMod_ModOp");
}

llvm::Constant * MulDivExprMod::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getSRem(left, right);
}

WC_END_NAMESPACE
