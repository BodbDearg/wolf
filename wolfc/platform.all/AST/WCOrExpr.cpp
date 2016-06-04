#include "WCAndExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
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

OrExpr * OrExpr::parse(const Token *& tokenPtr) {
    // Parse the initial expression
    AndExpr * andExpr = AndExpr::parse(tokenPtr);
    WC_GUARD(andExpr, nullptr);
    
    // See if there is an 'or' for logical or
    if (tokenPtr->type == TokenType::kOr) {
        // Or expression with or. Skip the 'or'
        ++tokenPtr;
        
        // Parse the following and expression and create the AST node
        OrExpr * orExpr = OrExpr::parse(tokenPtr);
        WC_GUARD(orExpr, nullptr);
        return new OrExprOr(*andExpr, *orExpr);
    }

    // Basic no-op expression
    return new OrExprNoOp(*andExpr);
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

llvm::Value * OrExprNoOp::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool OrExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & OrExprNoOp::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * OrExprNoOp::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
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

llvm::Value * OrExprOr::generateCode(const CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Left side must evaluate to a boolean:
    const DataType & leftType = mLeftExpr.getDataType();
    
    if (!leftType.equals(boolType)) {
        compileError("Left side of logical 'or' expression must evaluate to 'bool', not '%s'!", leftType.name());
        return nullptr;
    }
    
    // Right side must evaluate to a boolean:
    const DataType & rightType = mRightExpr.getDataType();
    
    if (!rightType.equals(boolType)) {
        compileError("Right side of logical 'or' expression must evaluate to 'bool', not '%s'!", rightType.name());
        return nullptr;
    }

    // Evaluate left side:
    llvm::Value * leftValue = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.generateCode(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate and return bitwise and instruction:
    return cgCtx.irBuilder.CreateOr(rightValue, leftValue);
}

bool OrExprOr::isLValue() const {
    return false;
}

const DataType & OrExprOr::getDataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * OrExprOr::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

WC_END_NAMESPACE
