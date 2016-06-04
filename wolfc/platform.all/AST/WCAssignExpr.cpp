#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCOrExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AssignExpr
//-----------------------------------------------------------------------------

bool AssignExpr::peek(const Token * tokenPtr) {
    return OrExpr::peek(tokenPtr);
}

AssignExpr * AssignExpr::parse(const Token *& tokenPtr) {
    // Parse the initial expression
    OrExpr * orExpr = OrExpr::parse(tokenPtr);
    WC_GUARD(orExpr, nullptr);
    
    // See if there is a '=' for assignment
    if (tokenPtr->type == TokenType::kEquals) {
        // Assign expression with assign. Skip the '='
        ++tokenPtr;
        
        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
        WC_GUARD(assignExpr, nullptr);
        return new AssignExprAssign(*orExpr, *assignExpr);
    }

    // Assign expression with no assign
    return new AssignExprNoAssign(*orExpr);
}

//-----------------------------------------------------------------------------
// AssignExprNoAssign
//-----------------------------------------------------------------------------

AssignExprNoAssign::AssignExprNoAssign(OrExpr & expr) : mExpr(expr) {
    expr.mParent = this;
}

const Token & AssignExprNoAssign::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AssignExprNoAssign::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * AssignExprNoAssign::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool AssignExprNoAssign::isLValue() const {
    return mExpr.isLValue();
}

const DataType & AssignExprNoAssign::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * AssignExprNoAssign::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// AssignExprAssign
//-----------------------------------------------------------------------------

AssignExprAssign::AssignExprAssign(OrExpr & leftExpr, AssignExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AssignExprAssign::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AssignExprAssign::getEndToken() const {
    return mRightExpr.getEndToken();
}

llvm::Value * AssignExprAssign::generateCode(const CodegenCtx & cgCtx) {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        compileError("Can't assign to an rvalue!");
        return nullptr;
    }
    
    // Evaluate left side
    llvm::Value * leftValue = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.generateCode(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    cgCtx.irBuilder.CreateStore(rightValue, leftValue);
    
    // The expression evalutes to the left expression, so return that
    return cgCtx.irBuilder.CreateLoad(leftValue);
}

bool AssignExprAssign::isLValue() const {
    return false;
}

const DataType & AssignExprAssign::getDataType() const {
    return mLeftExpr.getDataType();
}

llvm::Value * AssignExprAssign::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

WC_END_NAMESPACE
