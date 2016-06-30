#include "WCMulDivExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"
#include "WCUnaryExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// MulDivExpr
//-----------------------------------------------------------------------------

bool MulDivExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

MulDivExpr * MulDivExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    UnaryExpr * unaryExpr = UnaryExpr::parse(tokenPtr, alloc);
    WC_GUARD(unaryExpr, nullptr);
    
    // See if '*' or '/' following:
    if (tokenPtr->type == TokenType::kAsterisk) {
        // Mul operation: Skip '*'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        MulDivExpr * mulDivExpr = MulDivExpr::parse(tokenPtr, alloc);
        WC_GUARD(mulDivExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, MulDivExprMul, *unaryExpr, *mulDivExpr);
    }
    else if (tokenPtr->type == TokenType::kSlash) {
        // Div operation: Skip '/'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        MulDivExpr * mulDivExpr = MulDivExpr::parse(tokenPtr, alloc);
        WC_GUARD(mulDivExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, MulDivExprDiv, *unaryExpr, *mulDivExpr);
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, MulDivExprNoOp, *unaryExpr);
}

//-----------------------------------------------------------------------------
// MulDivExprNoOp
//-----------------------------------------------------------------------------

MulDivExprNoOp::MulDivExprNoOp(UnaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & MulDivExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & MulDivExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool MulDivExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & MulDivExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * MulDivExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * MulDivExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// MulDivExprMul
//-----------------------------------------------------------------------------

MulDivExprMul::MulDivExprMul(UnaryExpr & leftExpr, MulDivExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & MulDivExprMul::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulDivExprMul::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulDivExprMul::isLValue() const {
    return false;
}

const DataType & MulDivExprMul::dataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * MulDivExprMul::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * MulDivExprMul::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateMul(left, right, "MulDivExprMul_MulOp");
}

bool MulDivExprMul::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// MulDivExprDiv
//-----------------------------------------------------------------------------

MulDivExprDiv::MulDivExprDiv(UnaryExpr & leftExpr, MulDivExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & MulDivExprDiv::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulDivExprDiv::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulDivExprDiv::isLValue() const {
    return false;
}

const DataType & MulDivExprDiv::dataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * MulDivExprDiv::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * MulDivExprDiv::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSDiv(left, right, "MulDivExprDiv_DivOp");
}

bool MulDivExprDiv::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
