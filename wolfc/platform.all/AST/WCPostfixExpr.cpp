#include "WCPostfixExpr.hpp"

#include "WCArrayDataType.hpp"
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
#include "WCPrimitiveType.hpp"
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
    
    // See if array lookup follows - '[' token:
    if (currentToken->type == TokenType::kLBrack) {
        // Skip '['
        ++currentToken;
        
        // Parse the assign expression for the array index
        AssignExpr * arrayIndexExpr = AssignExpr::parse(currentToken, alloc);
        WC_GUARD(arrayIndexExpr, nullptr);
        
        // Expect a closing ']'
        const Token & endToken = *currentToken;
        
        if (endToken.type != TokenType::kRBrack) {
            parseError(endToken, "Expected a closing ']' for array lookup expression!");
            return nullptr;
        }
        
        // Consume the ']' and return the array lookup expression
        ++currentToken;
        return WC_NEW_AST_NODE(alloc,
                               PostfixExprArrayLookup,
                               *expr,
                               *arrayIndexExpr,
                               endToken);
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

DataType & PostfixExprNoPostfix::dataType() {
    return mExpr.dataType();
}

bool PostfixExprNoPostfix::requiresStorage() const {
    return mExpr.requiresStorage();
}

llvm::Value * PostfixExprNoPostfix::getStorage() const {
    return mExpr.getStorage();
}

void PostfixExprNoPostfix::setStorage(llvm::Value & storage) {
    mExpr.setStorage(storage);
}

llvm::Value * PostfixExprNoPostfix::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PostfixExprNoPostfix::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PostfixExprNoPostfix::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
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

DataType & PostfixExprFuncCall::dataType() {
    Func * funcCalled = lookupFuncCalled();
    
    if (funcCalled) {
        return funcCalled->returnDataType();
    }
    
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
}

llvm::Value * PostfixExprFuncCall::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * PostfixExprFuncCall::codegenExprEval(CodegenCtx & cgCtx) {
    // Name of function to call:
    const char * funcName = nameOfFuncCalled();
    
    if (!funcName) {
        compileError("Function to call must be specified by a single identifier!");
        return nullptr;
    }
    
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
                         funcArgDataType.name().c_str(),
                         callArgDataType.name().c_str());
            
            return nullptr;
        }
    }
    
    // Generate the code for the arguments list of the function call:
    WC_GUARD(mFuncCall.codegenArgsListExprs(cgCtx), nullptr);
    
    // Call it: note if non void we have to give the value a name
    if (func->returnDataType().isVoid()) {
        return cgCtx.irBuilder.CreateCall(func->mLLVMFunc,
                                          mFuncCall.mArgListExprsValues);
    }
    
    return cgCtx.irBuilder.CreateCall(func->mLLVMFunc,
                                      mFuncCall.mArgListExprsValues,
                                      "PostfixExprFuncCall:ReturnVal");
}

llvm::Constant * PostfixExprFuncCall::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: some day, eventually, support compile time function evaluation
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cannot call functions in a constant expression!");
    return nullptr;
}

const char * PostfixExprFuncCall::nameOfFuncCalled() const {
    // TODO: support member function calls on this object (some day)
    // TODO: support member function calls on another object (some day)
    // TODO: support lambda calls (some day)
    // TODO: support built in functions on basic types (somenum.isNan() etc.)
    PrimaryExprIdentifier * funcNameIdentifier = dynamic_cast<PrimaryExprIdentifier*>(&mExpr);
    
    if (!funcNameIdentifier) {
        return nullptr;
    }
    
    return funcNameIdentifier->name();
}

Func * PostfixExprFuncCall::lookupFuncCalled() const {
    // TODO: can we avoid a string construct here?
    
    // Get the function name
    const char * funcName = nameOfFuncCalled();
    WC_GUARD(funcName, nullptr);
    
    // Lookup parent module - this should always exist
    const Module * parentModule = firstParentOfType<Module>();
    WC_ASSERT(parentModule);
    return parentModule->getFunc(funcName);
}

//-----------------------------------------------------------------------------
// PostfixExprArrayLookup
//-----------------------------------------------------------------------------

PostfixExprArrayLookup::PostfixExprArrayLookup(PrimaryExpr & arrayExpr,
                                               AssignExpr & indexExpr,
                                               const Token & endToken)
:
    mArrayExpr(arrayExpr),
    mIndexExpr(indexExpr),
    mEndToken(endToken)
{
    mArrayExpr.mParent = this;
    mIndexExpr.mParent = this;
}

const Token & PostfixExprArrayLookup::getStartToken() const {
    return mArrayExpr.getStartToken();
}

const Token & PostfixExprArrayLookup::getEndToken() const {
    return mEndToken;
}

bool PostfixExprArrayLookup::isLValue() const {
    return mArrayExpr.isLValue();
}

DataType & PostfixExprArrayLookup::dataType() {
    // Expect the array expression to have the array data type.
    // If it doesn't have this type then we don't know what the element type is..
    const ArrayDataType * arrayDataType = dynamic_cast<ArrayDataType*>(&mArrayExpr.dataType());
    WC_GUARD(arrayDataType, PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown));
    
    // Return the element data type
    return arrayDataType->mInnerType;
}

llvm::Value * PostfixExprArrayLookup::codegenAddrOf(CodegenCtx & cgCtx) {
    return codegenAddrOfArrayElem(cgCtx);
}

llvm::Value * PostfixExprArrayLookup::codegenExprEval(CodegenCtx & cgCtx) {
    // Codegen the address of the array element
    llvm::Value * arrayElemAddr = codegenAddrOfArrayElem(cgCtx);
    WC_GUARD(arrayElemAddr, nullptr);

    // Get the array datatype
    ArrayDataType * arrayDataType = getArrayDataTypeOrIssueError();
    WC_GUARD(arrayElemAddr, nullptr);
    
    // If the element type is not an array then load, otherwise return it
    if (arrayDataType->mInnerType.isArray()) {
        return arrayElemAddr;
    }
    
    return cgCtx.irBuilder.CreateLoad(arrayElemAddr);
}

llvm::Constant * PostfixExprArrayLookup::codegenExprConstEval(CodegenCtx & cgCtx) {
    #warning TODO: array lookup constant code generation
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

ArrayDataType * PostfixExprArrayLookup::getArrayDataTypeOrIssueError() {
    ArrayDataType * arrayDataType = dynamic_cast<ArrayDataType*>(&mArrayExpr.dataType());
    
    if (!arrayDataType) {
        compileError("Can't perform array indexing on an expression that is not an array!");
        return nullptr;
    }
    
    return arrayDataType;
}

llvm::Value * PostfixExprArrayLookup::codegenAddrOfArrayElem(CodegenCtx & cgCtx) {
    // Make sure the array is actually an array...
    // TODO: support this operator on custom types eventually
    if (!mArrayExpr.dataType().isArray()) {
        compileError("Can't perform array indexing on an expression that is not an array!");
        return nullptr;
    }
    
    // Codgen the address of the array first
    llvm::Value * arrayAddr = mArrayExpr.codegenAddrOf(cgCtx);
    WC_GUARD(arrayAddr, nullptr);
    
    // Codegen the expression for the array index
    llvm::Value * indexValue = mIndexExpr.codegenExprEval(cgCtx);
    WC_GUARD(indexValue, nullptr);
    
    // Get the value for the array address and return it:
    llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), 0);
    WC_ASSERT(zeroIndex);
    llvm::Value * arrayAddress = cgCtx.irBuilder.CreateGEP(arrayAddr, { zeroIndex, indexValue });
    WC_ASSERT(arrayAddress);
    return arrayAddress;
}

WC_END_NAMESPACE
