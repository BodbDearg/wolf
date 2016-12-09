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
    if (tokenPtr[0].type == TokenType::kLessThan &&
        tokenPtr[1].type == TokenType::kLessThan &&
        tokenPtr[2].type == TokenType::kEquals)
    {
        // '<<=' assign expression. Skip the '<<='
        tokenPtr += 3;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignLShift, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kGreaterThan &&
             tokenPtr[1].type == TokenType::kGreaterThan &&
             tokenPtr[2].type == TokenType::kEquals)
    {
        // '>>=' assign expression. Skip the '>>='
        tokenPtr += 3;
        
        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignArithRShift, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kGreaterThan &&
             tokenPtr[1].type == TokenType::kGreaterThan &&
             tokenPtr[2].type == TokenType::kGreaterThan &&
             tokenPtr[3].type == TokenType::kEquals)
    {
        // '>>>=' assign expression. Skip the '>>>='
        tokenPtr += 4;
        
        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignLogicRShift, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kVBar &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '|=' assign expression. Skip the '|='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignBOr, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kHat &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '^=' assign expression. Skip the '^='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignBXor, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kAmpersand &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '&=' assign expression. Skip the '&='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignBAnd, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kPlus &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '+=' assign expression. Skip the '+='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignAdd, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kMinus &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '-=' assign expression. Skip the '-='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignSub, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kAsterisk &&
             tokenPtr[1].type == TokenType::kEquals) 
    {
        // '*=' assign expression. Skip the '*='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignMul, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kSlash &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '/=' assign expression. Skip the '/='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignDiv, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kPercent &&
             tokenPtr[1].type == TokenType::kEquals)
    {
        // '%=' assign expression. Skip the '%='
        tokenPtr += 2;

        // Parse the following expression and create the AST node
        AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(assignExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AssignExprAssignMod, *ternaryExpr, *assignExpr);
    }
    else if (tokenPtr[0].type == TokenType::kEquals) {
        // Assign expression with simple '=' assign and no op. Skip the '='
        ++tokenPtr;
        
        // Parse the following expression and create the AST node
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
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
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
// AssignExprAssignLShift
//-----------------------------------------------------------------------------

AssignExprAssignLShift::AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignLShift::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateShl(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignArithRShift
//-----------------------------------------------------------------------------

AssignExprAssignArithRShift::AssignExprAssignArithRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignArithRShift::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateAShr(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignLogicRShift
//-----------------------------------------------------------------------------

AssignExprAssignLogicRShift::AssignExprAssignLogicRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignLogicRShift::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateLShr(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignBOr
//-----------------------------------------------------------------------------

AssignExprAssignBOr::AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignBOr::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateOr(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignBXor
//-----------------------------------------------------------------------------

AssignExprAssignBXor::AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignBXor::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateXor(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
}

//-----------------------------------------------------------------------------
// AssignExprAssignBAnd
//-----------------------------------------------------------------------------

AssignExprAssignBAnd::AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignBAnd::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);
    
    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateAnd(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);
    
    // The expression evalutes to the left expression, which now has the value of the
    // new value, so return that...
    return newValue;
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
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

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
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

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
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

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
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

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

//-----------------------------------------------------------------------------
// AssignExprAssignMod
//-----------------------------------------------------------------------------

AssignExprAssignMod::AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AssignExprAssignMod::codegenExprEval(CodegenCtx & cgCtx) {
    // Basic compile checks:
    WC_GUARD(compileCheckAssignIsLegal(), nullptr);

    // Evaluate left value and right side value
    llvm::Value * leftAddr = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftAddr, nullptr);
    llvm::Value * leftValue = cgCtx.irBuilder.CreateLoad(leftAddr);
    WC_ASSERT(leftValue);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);

    // Do the operation and store the result on the left
    llvm::Value * newValue = cgCtx.irBuilder.CreateSRem(leftValue, rightValue);
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(newValue, leftAddr);
    WC_ASSERT(storeInst);

    // The expression evalutes to the left expression, which now has the value of the 
    // new value, so return that...
    return newValue;
}

WC_END_NAMESPACE
