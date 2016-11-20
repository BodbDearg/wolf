#include "WCEqExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCRelExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// EqExpr
//-----------------------------------------------------------------------------

bool EqExpr::peek(const Token * tokenPtr) {
    return RelExpr::peek(tokenPtr);
}

EqExpr * EqExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    RelExpr * relExpr = RelExpr::parse(tokenPtr, alloc);
    WC_GUARD(relExpr, nullptr);
    
    // See what tokens follow:
    if (tokenPtr[0].type == TokenType::kEquals && tokenPtr[1].type == TokenType::kEquals) {
        ++tokenPtr;     // Skip '='
        ++tokenPtr;     // Skip '='
        
        EqExpr * eqExpr = EqExpr::parse(tokenPtr, alloc);
        WC_GUARD(eqExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, EqExprEq, *relExpr, *eqExpr);
    }
    else if (tokenPtr[0].type == TokenType::kExclamation && tokenPtr[1].type == TokenType::kEquals) {
        ++tokenPtr;     // Skip '!'
        ++tokenPtr;     // Skip '='
        
        EqExpr * eqExpr = EqExpr::parse(tokenPtr, alloc);
        WC_GUARD(eqExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, EqExprNeq, *relExpr, *eqExpr);
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, EqExprNoOp, *relExpr);
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

bool EqExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool EqExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & EqExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * EqExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * EqExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * EqExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
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

bool EqExprTwoOps::isLValue() {
    return false;
}

bool EqExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & EqExprTwoOps::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * EqExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of '==' or '!=' expression result!");
    return nullptr;
}

bool EqExprTwoOps::compileCheckBothExprsAreInt() const {
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
// EqExprEq
//-----------------------------------------------------------------------------

EqExprEq::EqExprEq(RelExpr & leftExpr, EqExpr & rightExpr) : EqExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * EqExprEq::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpEQ(left, right, "EqExprEq_CmpOp");
}

llvm::Constant * EqExprEq::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, left, right);
}

//-----------------------------------------------------------------------------
// EqExprNeq
//-----------------------------------------------------------------------------

EqExprNeq::EqExprNeq(RelExpr & leftExpr, EqExpr & rightExpr) : EqExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * EqExprNeq::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpNE(left, right, "EqExprNeq_CmpOp");
}

llvm::Constant * EqExprNeq::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, left, right);
}

WC_END_NAMESPACE
