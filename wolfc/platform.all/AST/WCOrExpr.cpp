#include "WCAndExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCOrExpr.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// OrExpr
//-----------------------------------------------------------------------------

bool OrExpr::peek(const Token * tokenPtr) {
    return AndExpr::peek(tokenPtr);
}

OrExpr * OrExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial expression
    AndExpr * andExpr = AndExpr::parse(tokenPtr, alloc);
    WC_GUARD(andExpr, nullptr);
    
    // See if there is an 'or' for logical or
    if (tokenPtr->type == TokenType::kOr) {
        // Or expression with or. Skip the 'or'
        ++tokenPtr;
        
        // Parse the following and expression and create the AST node
        OrExpr * orExpr = OrExpr::parse(tokenPtr, alloc);
        WC_GUARD(orExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, OrExprOr, *andExpr, *orExpr);
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(alloc, OrExprNoOp, *andExpr);
}

//-----------------------------------------------------------------------------
// OrExprNoOp
//-----------------------------------------------------------------------------

OrExprNoOp::OrExprNoOp(AndExpr & expr) : mExpr(expr) {
    expr.mParent = this;
}

const Token & OrExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & OrExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool OrExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & OrExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * OrExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * OrExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * OrExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// OrExprOr
//-----------------------------------------------------------------------------

OrExprOr::OrExprOr(AndExpr & leftExpr, OrExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & OrExprOr::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & OrExprOr::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool OrExprOr::isLValue() const {
    return false;
}

const DataType & OrExprOr::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * OrExprOr::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * OrExprOr::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Left side must evaluate to a boolean:
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(boolType)) {
        compileError("Left side of logical 'or' expression must evaluate to 'bool', not '%s'!", leftType.name());
        return nullptr;
    }
    
    // Right side must evaluate to a boolean:
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(boolType)) {
        compileError("Right side of logical 'or' expression must evaluate to 'bool', not '%s'!", rightType.name());
        return nullptr;
    }

    // Evaluate left side:
    llvm::Value * leftValue = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate and return bitwise and instruction:
    return cgCtx.irBuilder.CreateOr(rightValue, leftValue);
}

llvm::Constant * OrExprOr::codegenExprConstEval(CodegenCtx & cgCtx) {
    #warning TODO: implement constant evaluation
    WC_UNUSED_PARAM(cgCtx);
    compileError("Constant evaluation supported yet for this tyoe of expression!");
    return nullptr;
}

WC_END_NAMESPACE
