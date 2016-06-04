#include "WCAndExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AndExpr
//-----------------------------------------------------------------------------

bool AndExpr::peek(const Token * tokenPtr) {
    return BinaryExpr::peek(tokenPtr);
}

AndExpr * AndExpr::parse(const Token *& tokenPtr) {
    // Parse the initial expression
    BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
    WC_GUARD(binaryExpr, nullptr);
    
    // See if there is an 'and' for logical and
    if (tokenPtr->type == TokenType::kAnd) {
        // And expression with and. Skip the 'and'
        ++tokenPtr;
        
        // Parse the following and expression and create the AST node
        AndExpr * andExpr = AndExpr::parse(tokenPtr);
        WC_GUARD(andExpr, nullptr);
        return new AndExprAnd(*binaryExpr, *andExpr);
    }

    // And expression with no and
    return new AndExprNoAnd(*binaryExpr);
}

//-----------------------------------------------------------------------------
// AndExprNoAnd
//-----------------------------------------------------------------------------

AndExprNoAnd::AndExprNoAnd(BinaryExpr & expr) : mExpr(expr) {
    expr.mParent = this;
}

const Token & AndExprNoAnd::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AndExprNoAnd::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * AndExprNoAnd::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool AndExprNoAnd::isLValue() const {
    return mExpr.isLValue();
}

const DataType & AndExprNoAnd::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * AndExprNoAnd::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// AndExprAnd
//-----------------------------------------------------------------------------

AndExprAnd::AndExprAnd(BinaryExpr & leftExpr, AndExpr & rightExpr) :
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

llvm::Value * AndExprAnd::generateCode(const CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Left side must evaluate to a boolean:
    const DataType & leftType = mLeftExpr.getDataType();
    
    if (!leftType.equals(boolType)) {
        compileError("Left side of logical 'and' expression must evaluate to 'bool', not '%s'!", leftType.name());
        return nullptr;
    }
    
    // Right side must evaluate to a boolean:
    const DataType & rightType = mRightExpr.getDataType();
    
    if (!rightType.equals(boolType)) {
        compileError("Right side of logical 'and' expression must evaluate to 'bool', not '%s'!", rightType.name());
        return nullptr;
    }

    // Evaluate left side:
    llvm::Value * leftValue = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.generateCode(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate and return bitwise and instruction:
    return cgCtx.irBuilder.CreateAnd(rightValue, leftValue);
}

bool AndExprAnd::isLValue() const {
    return false;
}

const DataType & AndExprAnd::getDataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * AndExprAnd::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

WC_END_NAMESPACE
