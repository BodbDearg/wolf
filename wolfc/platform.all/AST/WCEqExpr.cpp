#include "WCEqExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCRelExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// EqExpr
//-----------------------------------------------------------------------------

bool EqExpr::peek(const Token * tokenPtr) {
    return RelExpr::peek(tokenPtr);
}

EqExpr * EqExpr::parse(const Token *& tokenPtr) {
    RelExpr * relExpr = RelExpr::parse(tokenPtr);
    WC_GUARD(relExpr, nullptr);
    
    // See what tokens follow:
    if (tokenPtr->type == TokenType::kEquals) {
        ++tokenPtr;     // Skip '='
        
        if (tokenPtr->type != TokenType::kEquals) {
            parseError(*tokenPtr, "Expected another '=' following '='!");
            return nullptr;
        }
        
        ++tokenPtr;     // Skip '='
        EqExpr * eqExpr = EqExpr::parse(tokenPtr);
        WC_GUARD(eqExpr, nullptr);
        return new EqExprEq(*relExpr, *eqExpr);
    }
    else if (tokenPtr->type == TokenType::kExclamation) {
        ++tokenPtr;     // Skip '!'
        
        if (tokenPtr->type != TokenType::kEquals) {
            parseError(*tokenPtr, "Expected another '=' following '!'!");
            return nullptr;
        }
        
        ++tokenPtr;     // Skip '='
        EqExpr * eqExpr = EqExpr::parse(tokenPtr);
        WC_GUARD(eqExpr, nullptr);
        return new EqExprNeq(*relExpr, *eqExpr);
    }

    // Basic no-op expression:
    return new EqExprNoOp(*relExpr);
}

//-----------------------------------------------------------------------------
// EqExprNoOp
//-----------------------------------------------------------------------------

EqExprNoOp::EqExprNoOp(RelExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & EqExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & EqExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool EqExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & EqExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * EqExprNoOp::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * EqExprNoOp::codegenExprEval(const CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// EqExprTwoOps
//-----------------------------------------------------------------------------

EqExprTwoOps::EqExprTwoOps(RelExpr & leftExpr, EqExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & EqExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & EqExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool EqExprTwoOps::isLValue() const {
    return false;
}

const DataType & EqExprTwoOps::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * EqExprTwoOps::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

bool EqExprTwoOps::compileCheckBothExprsAreInt() const {
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
// EqExprEq
//-----------------------------------------------------------------------------

EqExprEq::EqExprEq(RelExpr & leftExpr, EqExpr & rightExpr) : EqExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * EqExprEq::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpEQ(left, right, "EqExprEq_CmpOp");
}

//-----------------------------------------------------------------------------
// EqExprNeq
//-----------------------------------------------------------------------------

EqExprNeq::EqExprNeq(RelExpr & leftExpr, EqExpr & rightExpr) : EqExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * EqExprNeq::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpNE(left, right, "EqExprNeq_CmpOp");
}

WC_END_NAMESPACE
