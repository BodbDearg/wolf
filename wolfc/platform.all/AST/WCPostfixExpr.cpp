#include "WCPostfixExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCFuncInvocation.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
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
    
    // See if function invocation follows:
    if (FuncInvocation::peek(currentToken)) {
        FuncInvocation * funcInvocation = FuncInvocation::parse(currentToken, alloc);
        WC_GUARD(funcInvocation, nullptr);
        return WC_NEW_AST_NODE(alloc, PostfixExprFuncInvocation, *expr, *funcInvocation);
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

PostfixExprFuncInvocation::PostfixExprFuncInvocation(PrimaryExpr & expr, FuncInvocation & funcInvocation) :
    mExpr(expr),
    mFuncInvocation(funcInvocation)
{
    mExpr.mParent = this;
    mFuncInvocation.mParent = this;
}

const Token & PostfixExprFuncInvocation::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprFuncInvocation::getEndToken() const {
    return mFuncInvocation.getEndToken();
}

bool PostfixExprFuncInvocation::isLValue() const {
    return false;
}

const DataType & PostfixExprFuncInvocation::dataType() const {
    // TODO: support return types other than void
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kVoid);
}

llvm::Value * PostfixExprFuncInvocation::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * PostfixExprFuncInvocation::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: support member function calls (some day)
    PrimaryExprIdentifier * funcNameIdentifier = dynamic_cast<PrimaryExprIdentifier*>(&mExpr);
    
    if (!funcNameIdentifier) {
        compileError("Function to call must be specified by a single identifier!");
        return nullptr;
    }
    
    const char * funcName = funcNameIdentifier->name();
    WC_ASSERT(funcName);
    
    // Get the function to call:
    llvm::Constant * func = cgCtx.module.getFunction(funcName);
    
    if (!func) {
        compileError("No such function to call: %s", funcName);
        return nullptr;
    }
    
    // TODO: support passing arguments
    // Call it:
    return cgCtx.irBuilder.CreateCall(func, {});
}

WC_END_NAMESPACE
