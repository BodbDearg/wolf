#include "WCAssignExpr.hpp"

#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCTernaryExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AssignExpr
//-----------------------------------------------------------------------------

bool AssignExpr::peek(const Token * tokenPtr) {
    return TernaryExpr::peek(tokenPtr);
}

AssignExpr * AssignExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial expression
    TernaryExpr * ternaryExpr = TernaryExpr::parse(tokenPtr, alloc);
    WC_GUARD(ternaryExpr, nullptr);
    
    // See if there is a '=' for assignment
    if (tokenPtr->type == TokenType::kEquals) {
        // Assign expression with assign. Skip the '='
        ++tokenPtr;
        
        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssign, *ternaryExpr, *assignExpr);
    }

    // Assign expression with no assign
    return WC_NEW_AST_NODE(alloc, AssignExprNoAssign, *ternaryExpr);
}

//-----------------------------------------------------------------------------
// AssignExprNoAssign
//-----------------------------------------------------------------------------

AssignExprNoAssign::AssignExprNoAssign(TernaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & AssignExprNoAssign::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AssignExprNoAssign::getEndToken() const {
    return mExpr.getEndToken();
}

bool AssignExprNoAssign::isLValue() const {
    return mExpr.isLValue();
}

DataType & AssignExprNoAssign::dataType() {
    return mExpr.dataType();
}

bool AssignExprNoAssign::requiresStorage() const {
    return mExpr.requiresStorage();
}

llvm::Value * AssignExprNoAssign::getStorage() const {
    return mExpr.getStorage();
}

void AssignExprNoAssign::setStorage(llvm::Value & storage) {
    mExpr.setStorage(storage);
}

llvm::Value * AssignExprNoAssign::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AssignExprNoAssign::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * AssignExprNoAssign::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// AssignExprAssign
//-----------------------------------------------------------------------------

AssignExprAssign::AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
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

bool AssignExprAssign::isLValue() const {
    return false;
}

DataType & AssignExprAssign::dataType() {
    return mLeftExpr.dataType();
}

llvm::Value * AssignExprAssign::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * AssignExprAssign::codegenExprEval(CodegenCtx & cgCtx) {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        compileError("Can't assign to an rvalue!");
        return nullptr;
    }
    
    // TODO: support auto type promotion and the lark
    // The data type of the right must match the left
    const DataType & leftDataType = mLeftExpr.dataType();
    const DataType & rightDataType = mRightExpr.dataType();
    
    // TODO: print variable name here
    if (!leftDataType.equals(rightDataType)) {
        compileError("Can't assign expression of type '%s' to variable of type '%s'!",
                     rightDataType.name().c_str(),
                     leftDataType.name().c_str());
        
        return nullptr;
    }
    
    // Evaluate left side
    llvm::Value * leftValue = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    cgCtx.irBuilder.CreateStore(rightValue, leftValue);
    
    // The expression evalutes to the left expression, so return that
    return cgCtx.irBuilder.CreateLoad(leftValue);
}

llvm::Constant * AssignExprAssign::codegenExprConstEval(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Assign is not allowed in a constant expression!");
    return nullptr;
}

WC_END_NAMESPACE
