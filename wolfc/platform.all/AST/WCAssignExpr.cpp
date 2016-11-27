#include "WCAssignExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCTernaryExpr.hpp"

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

    // See if we might have a '+=' or '-=' etc. or a plain assign ('=') ahead:
    if (tokenPtr->type == TokenType::kPlus && 
        tokenPtr[1].type == TokenType::kEquals) 
    {
        // '+=' assign expression. Skip the '+='
        ++tokenPtr;
        ++tokenPtr;

        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignAdd, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr->type == TokenType::kMinus && 
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '-=' assign expression. Skip the '-='
        ++tokenPtr;
        ++tokenPtr;

        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignSub, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr->type == TokenType::kAsterisk &&
             tokenPtr[1].type == TokenType::kEquals) 
    {
        // '*=' assign expression. Skip the '*='
        ++tokenPtr;
        ++tokenPtr;

        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignMul, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr->type == TokenType::kSlash &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '/=' assign expression. Skip the '/='
        ++tokenPtr;
        ++tokenPtr;

        // Parse the following assign expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignDiv, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr->type == TokenType::kEquals) {
        // Assign expression with simple '=' assign and no op. Skip the '='
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

bool AssignExprNoAssign::isLValue() {
    return mExpr.isLValue();
}

bool AssignExprNoAssign::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & AssignExprNoAssign::dataType() {
    return mExpr.dataType();
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
// AssignExprAssignBase
//-----------------------------------------------------------------------------

AssignExprAssignBase::AssignExprAssignBase(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AssignExprAssignBase::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AssignExprAssignBase::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AssignExprAssignBase::isLValue() {
    return false;
}

bool AssignExprAssignBase::isConstExpr() {
    // TODO: eventually we could relax this perhaps and allow for full constant evaluation of a
    // function provided it does not rely on something that can externally vary.
    // For now however, you must use functional type programming if you want constant evaluation.
    return false;
}

DataType & AssignExprAssignBase::dataType() {
    // TODO: Handle auto type promotion eventually
    return mLeftExpr.dataType();
}

llvm::Value * AssignExprAssignBase::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an assign type ('=') expression result!");
    return nullptr;
}

llvm::Constant * AssignExprAssignBase::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: eventually we could relax this perhaps and allow for full constant evaluation of a
    // function provided it does not rely on something that can externally vary.
    // For now however, you must use functional type programming if you want constant evaluation.
    WC_UNUSED_PARAM(cgCtx);
    compileError("Assign is not allowed in a constant expression!");
    return nullptr;
}

bool AssignExprAssignBase::compileCheckAssignIsLegal() {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        compileError("Can't assign to an rvalue!");
        return false;
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

        return false;
    }

    // All good!
    return true;
}

//-----------------------------------------------------------------------------
// AssignExprAssign
//-----------------------------------------------------------------------------

AssignExprAssign::AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssign::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), false);
    
    // Evaluate left address and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(rightValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the 
    // right expression, so return that...
    return rightValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignAdd
//-----------------------------------------------------------------------------

AssignExprAssignAdd::AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignAdd::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), false);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);

    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateAdd(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignSub
//-----------------------------------------------------------------------------

AssignExprAssignSub::AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignSub::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), false);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);

    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateSub(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignMul
//-----------------------------------------------------------------------------

AssignExprAssignMul::AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignMul::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), false);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);

    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateMul(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignDiv
//-----------------------------------------------------------------------------

AssignExprAssignDiv::AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignDiv::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), false);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);

    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateSDiv(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newValue;
}

WC_END_NAMESPACE
