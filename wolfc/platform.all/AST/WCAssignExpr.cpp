#include "WCAssignExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool AssignExpr::peek(const Token * tokenPtr) {
    return BinaryExpr::peek(tokenPtr);
}

AssignExpr * AssignExpr::parse(const Token *& tokenPtr) {
    // Parse the initial binary expression
    BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
    WC_GUARD(binaryExpr, nullptr);
    
    // See if there is a '=' for assignment
    if (tokenPtr->type == TokenType::kEquals) {
        // Assign expression with assign. Skip the '='
        ++tokenPtr;
        
        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
        WC_GUARD(assignExpr, nullptr);
        return new AssignExprAssign(*binaryExpr, *assignExpr);
    }

    // Assign expression with no assign
    return new AssignExprNoAssign(*binaryExpr);
}

AssignExprNoAssign::AssignExprNoAssign(BinaryExpr & expr) : mExpr(expr) {
    expr.mParent = this;
}

llvm::Value * AssignExprNoAssign::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool AssignExprNoAssign::isLValue() const {
    return mExpr.isLValue();
}

llvm::Value * AssignExprNoAssign::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

AssignExprAssign::AssignExprAssign(BinaryExpr & leftExpr, AssignExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

llvm::Value * AssignExprAssign::generateCode(const CodegenCtx & cgCtx) {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        // TODO: we need line number info here!!
        error("Can't assign to an rvalue!");
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
    return leftValue;
}

bool AssignExprAssign::isLValue() const {
    return false;
}

llvm::Value * AssignExprAssign::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

WC_END_NAMESPACE
