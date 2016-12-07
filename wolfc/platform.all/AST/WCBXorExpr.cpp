#include "WCBXorExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCBAndExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BXorExpr
//-----------------------------------------------------------------------------

bool BXorExpr::peek(const Token * tokenPtr) {
    return BAndExpr::peek(tokenPtr);
}

BXorExpr * BXorExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    BAndExpr * leftExpr = BAndExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);

    // See if '^' following:
    if (tokenPtr->type == TokenType::kHat && BAndExpr::peek(tokenPtr + 1)) {
        // Bitwise and operation: Skip '&'
        ++tokenPtr;

        // Parse following expr and return combined expr
        BXorExpr * rightExpr = BXorExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, BXorExprXor, *leftExpr, *rightExpr);
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, BXorExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// BXorExprNoOp
//-----------------------------------------------------------------------------

BXorExprNoOp::BXorExprNoOp(BAndExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & BXorExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & BXorExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool BXorExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool BXorExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & BXorExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * BXorExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * BXorExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * BXorExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// BXorExprXor
//-----------------------------------------------------------------------------

BXorExprXor::BXorExprXor(BAndExpr & leftExpr, BXorExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & BXorExprXor::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & BXorExprXor::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool BXorExprXor::isLValue() {
    return false;
}

bool BXorExprXor::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & BXorExprXor::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * BXorExprXor::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '^' (bitwise xor) operator result!");
    return nullptr;
}

llvm::Value * BXorExprXor::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateXor(left, right, "BXorExprXor_XorOp");
}

llvm::Constant * BXorExprXor::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getXor(left, right);
}

bool BXorExprXor::compileCheckBothExprsAreInt() const {
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

WC_END_NAMESPACE
