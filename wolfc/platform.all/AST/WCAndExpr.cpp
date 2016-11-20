#include "WCAndExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCNotExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AndExpr
//-----------------------------------------------------------------------------

bool AndExpr::peek(const Token * tokenPtr) {
    return NotExpr::peek(tokenPtr);
}

AndExpr * AndExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial expression
    NotExpr * notExpr = NotExpr::parse(tokenPtr, alloc);
    WC_GUARD(notExpr, nullptr);
    
    // See if there is an 'and' for logical and
    if (tokenPtr->type == TokenType::kAnd) {
        // And expression with and. Skip the 'and'
        ++tokenPtr;
        
        // Parse the following and expression and create the AST node
        AndExpr * andExpr = AndExpr::parse(tokenPtr, alloc);
        WC_GUARD(andExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AndExprAnd, *notExpr, *andExpr);
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(alloc, AndExprNoOp, *notExpr);
}

//-----------------------------------------------------------------------------
// AndExprNoOp
//-----------------------------------------------------------------------------

AndExprNoOp::AndExprNoOp(NotExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & AndExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AndExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool AndExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool AndExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & AndExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * AndExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AndExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * AndExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// AndExprAnd
//-----------------------------------------------------------------------------

AndExprAnd::AndExprAnd(NotExpr & leftExpr, AndExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AndExprAnd::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AndExprAnd::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AndExprAnd::isLValue() {
    return false;
}

bool AndExprAnd::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & AndExprAnd::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * AndExprAnd::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'and' operator result!");
    return nullptr;
}

llvm::Value * AndExprAnd::codegenExprEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    compileCheckBothExprsAreBool();

    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions and create the instruction:
    llvm::Value * leftValue = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    return cgCtx.irBuilder.CreateAnd(rightValue, leftValue);
}

llvm::Constant * AndExprAnd::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    compileCheckBothExprsAreBool();
    
    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions and create the instruction:
    llvm::Constant * leftValue = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Constant * rightValue = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    return llvm::ConstantExpr::getAnd(rightValue, leftValue);
}

bool AndExprAnd::compileCheckBothExprsAreBool() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.isBool()) {
        compileError("Left side of logical 'and' expression must evaluate to 'bool', not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.isBool()) {
        compileError("Right side of logical 'and' expression must evaluate to 'bool', not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
