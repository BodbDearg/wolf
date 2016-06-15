#include "WCAndExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCNotExpr.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

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
    expr.mParent = this;
}

const Token & AndExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AndExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool AndExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & AndExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * AndExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AndExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
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

bool AndExprAnd::isLValue() const {
    return false;
}

const DataType & AndExprAnd::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * AndExprAnd::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * AndExprAnd::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Left side must evaluate to a boolean:
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(boolType)) {
        compileError("Left side of logical 'and' expression must evaluate to 'bool', not '%s'!", leftType.name());
        return nullptr;
    }
    
    // Right side must evaluate to a boolean:
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(boolType)) {
        compileError("Right side of logical 'and' expression must evaluate to 'bool', not '%s'!", rightType.name());
        return nullptr;
    }

    // Evaluate left side:
    llvm::Value * leftValue = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate and return bitwise and instruction:
    return cgCtx.irBuilder.CreateAnd(rightValue, leftValue);
}

WC_END_NAMESPACE
