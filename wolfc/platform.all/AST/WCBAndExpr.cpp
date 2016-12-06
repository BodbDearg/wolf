#include "WCBAndExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCNotOrBNotExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BAndExpr
//-----------------------------------------------------------------------------

bool BAndExpr::peek(const Token * tokenPtr) {
    return NotOrBNotExpr::peek(tokenPtr);
}

BAndExpr * BAndExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    NotOrBNotExpr * leftExpr = NotOrBNotExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);

    // See if '&' following:
    if (tokenPtr->type == TokenType::kAmpersand && BAndExpr::peek(tokenPtr + 1)) {
        // Bitwise and operation: Skip '&'
        ++tokenPtr;

        // Parse following expr and return combined expr
        BAndExpr * rightExpr = BAndExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, BAndExprBAnd, *leftExpr, *rightExpr);
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, BAndExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// BAndExprNoOp
//-----------------------------------------------------------------------------

BAndExprNoOp::BAndExprNoOp(NotOrBNotExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & BAndExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & BAndExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool BAndExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool BAndExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & BAndExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * BAndExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * BAndExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * BAndExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// BAndExprBAnd
//-----------------------------------------------------------------------------

BAndExprBAnd::BAndExprBAnd(NotOrBNotExpr & leftExpr, BAndExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & BAndExprBAnd::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & BAndExprBAnd::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool BAndExprBAnd::isLValue() {
    return false;
}

bool BAndExprBAnd::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & BAndExprBAnd::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * BAndExprBAnd::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '&' (bitwise and) operator result!");
    return nullptr;
}

llvm::Value * BAndExprBAnd::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateAnd(left, right, "BAndExprBAnd_AndOp");
}

llvm::Constant * BAndExprBAnd::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getAnd(left, right);
}

bool BAndExprBAnd::compileCheckBothExprsAreInt() const {
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
