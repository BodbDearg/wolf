#include "WCPostfixExpr.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCFunc.hpp"
#include "WCFuncArg.hpp"
#include "WCFuncCall.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PostfixExpr
//-----------------------------------------------------------------------------

bool PostfixExpr::peek(const Token * currentToken) {
    return PrimaryExpr::peek(currentToken);
}

PostfixExpr * PostfixExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // Parse the initial primary
    PrimaryExpr * expr = PrimaryExpr::parse(currentToken, alloc);
    WC_GUARD(expr, nullptr);
    
    // See if function call follows:
    if (FuncCall::peek(currentToken)) {
        FuncCall * funcCall = FuncCall::parse(currentToken, alloc);
        WC_GUARD(funcCall, nullptr);
        return WC_NEW_AST_NODE(alloc, PostfixExprFuncCall, *expr, *funcCall);
    }
    
    // No postfix, basic primary expression:
    return WC_NEW_AST_NODE(alloc, PostfixExprNoPostfix, *expr);
}

//-----------------------------------------------------------------------------
// PostfixExprNoPostfix
//-----------------------------------------------------------------------------

PostfixExprNoPostfix::PostfixExprNoPostfix(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & PostfixExprNoPostfix::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprNoPostfix::getEndToken() const {
    return mExpr.getEndToken();
}

bool PostfixExprNoPostfix::isLValue() const {
    return mExpr.isLValue();
}

const DataType & PostfixExprNoPostfix::dataType() const {
    return mExpr.dataType();
}

llvm::Value * PostfixExprNoPostfix::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PostfixExprNoPostfix::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// PostfixExprFuncInvocation
//-----------------------------------------------------------------------------

PostfixExprFuncCall::PostfixExprFuncCall(PrimaryExpr & expr, FuncCall & funcCall) :
    mExpr(expr),
    mFuncCall(funcCall)
{
    mExpr.mParent = this;
    mFuncCall.mParent = this;
}

const Token & PostfixExprFuncCall::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprFuncCall::getEndToken() const {
    return mFuncCall.getEndToken();
}

bool PostfixExprFuncCall::isLValue() const {
    return false;
}

const DataType & PostfixExprFuncCall::dataType() const {
    // TODO: support return types other than void
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kVoid);
}

llvm::Value * PostfixExprFuncCall::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * PostfixExprFuncCall::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: support member function calls on this object (some day)
    // TODO: support member function calls on another object (some day)
    // TODO: support lambda calls (some day)
    // TODO: support built in functions on basic types (somenum.isNan() etc.)
    PrimaryExprIdentifier * funcNameIdentifier = dynamic_cast<PrimaryExprIdentifier*>(&mExpr);
    
    if (!funcNameIdentifier) {
        compileError("Function to call must be specified by a single identifier!");
        return nullptr;
    }
    
    // Name of function to call:
    const char * funcName = funcNameIdentifier->name();
    WC_ASSERT(funcName);
    
    // Get the function to call:
    Func * func = cgCtx.module.getFunc(funcName);
    
    if (!func) {
        compileError("No such function named '%s' exists to call!", funcName);
        return nullptr;
    }
    
    // Verify the number of arguments being given to the function is correct:
    std::vector<FuncArg*> funcArgs;
    func->getArgs(funcArgs);
    
    std::vector<AssignExpr*> callArgs;
    mFuncCall.getArgs(callArgs);
    size_t numFuncArgs = funcArgs.size();
    
    if (numFuncArgs != callArgs.size()) {
        compileError("Invalid number of argumenst for function call! Expected %zu instead of %zu!",
                     numFuncArgs,
                     callArgs.size());
        
        return nullptr;
    }
    
    // Verify the type of each argument is correct:
    for (size_t i = 0; i < numFuncArgs; ++i) {
        const DataType & funcArgDataType = funcArgs[i]->dataType();
        const DataType & callArgDataType = callArgs[i]->dataType();
        
        // TODO: support auto promotion
        if (!funcArgDataType.equals(callArgDataType)) {
            compileError("Type for arg number '%zu' in function call is invalid! Expected '%s' "
                         "but instead got '%s'!",
                         i + 1,
                         funcArgDataType.name(),
                         callArgDataType.name());
            
            return nullptr;
        }
    }
    
    // Generate the code for the arguments list of the function call:
    WC_GUARD(mFuncCall.codegenArgsListExprs(cgCtx), nullptr);
    
    // Call it:
    return cgCtx.irBuilder.CreateCall(func->mLLVMFunc, mFuncCall.mArgListExprsValues);
}

WC_END_NAMESPACE
