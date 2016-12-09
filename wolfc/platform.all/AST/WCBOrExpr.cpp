#include "WCBOrExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCBXorExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BOrExpr
//-----------------------------------------------------------------------------

bool BOrExpr::peek(const Token * tokenPtr) {
    return BXorExpr::peek(tokenPtr);
}

BOrExpr * BOrExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    BXorExpr * leftExpr = BXorExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);

    // See if '|' following:
    if (tokenPtr->type == TokenType::kVBar && BOrExpr::peek(tokenPtr + 1)) {
        // Bitwise or operation: Skip '|'
        ++tokenPtr;

        // Parse following expr and return combined expr
        BOrExpr * rightExpr = BOrExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, BOrExprOr, *leftExpr, *rightExpr);
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, BOrExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// BOrExprNoOp
//-----------------------------------------------------------------------------

BOrExprNoOp::BOrExprNoOp(BXorExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & BOrExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & BOrExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool BOrExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool BOrExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & BOrExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * BOrExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * BOrExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * BOrExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// BOrExprOr
//-----------------------------------------------------------------------------

BOrExprOr::BOrExprOr(BXorExpr & leftExpr, BOrExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & BOrExprOr::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & BOrExprOr::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool BOrExprOr::isLValue() {
    return false;
}

bool BOrExprOr::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & BOrExprOr::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * BOrExprOr::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '|' (bitwise or) operator result!");
    return nullptr;
}

llvm::Value * BOrExprOr::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateOr(left, right, "BXorExprOr_OrOp");
}

llvm::Constant * BOrExprOr::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getOr(left, right);
}

bool BOrExprOr::compileCheckBothExprsAreInt() const {
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
