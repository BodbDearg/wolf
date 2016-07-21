#include "WCFunc.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCFuncArg.hpp"
#include "WCFuncArgList.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCPrimitiveType.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"
#include <set>

WC_BEGIN_NAMESPACE

bool Func::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Must be a function ahead:
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the function identifier:
    Identifier * identifier = Identifier::parse(tokenPtr, alloc);
    WC_GUARD(identifier, nullptr);
    
    // Expect '('
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "'(' expected following function name!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip '('
    
    // See if an argument list follows:
    FuncArgList * argList = nullptr;
    
    if (FuncArgList::peek(tokenPtr)) {
        argList = FuncArgList::parse(tokenPtr, alloc);
        WC_GUARD(argList, nullptr);
    }
    
    // Expect ')'
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "')' expected to close args list of function!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip ')'
    
    // TODO: Support implicitly determining the return type. This eventually should be optional.
    // Expect '->' following function params for return type
    if (tokenPtr->type != TokenType::kOpArrow) {
        parseError(*tokenPtr, "Expected '->' following function params to specify return type!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip '->'
    
    // TODO: Support implicitly determining the return type. This eventually should be optional.
    // Parse the return type:
    PrimitiveType * returnType = PrimitiveType::parse(tokenPtr, alloc);
    WC_GUARD(returnType, nullptr);
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(tokenPtr, alloc);
    WC_GUARD(scope, nullptr);
    
    // Must be terminated by an 'end' token
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate function definition!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = tokenPtr;
    ++tokenPtr;
    
    // Done: return the parsed function
    return WC_NEW_AST_NODE(alloc,
                           Func,
                           *startToken,
                           *identifier,
                           argList,
                           *returnType,
                           *scope,
                           *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           FuncArgList * argList,
           PrimitiveType & returnType,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mArgList(argList),
    mReturnType(returnType),
    mScope(scope),
    mEndToken(endToken)
{
    mIdentifier.mParent = this;
    
    if (mArgList) {
        mArgList->mParent = this;
    }
    
    mScope.mParent = this;
}

const Token & Func::getStartToken() const {
    return mStartToken;
}

const Token & Func::getEndToken() const {
    return mEndToken;
}

const char * Func::name() const {
    return mIdentifier.name();
}

size_t Func::numArgs() const {
    WC_GUARD(mArgList, 0);
    return mArgList->numArgs();
}

void Func::getArgs(std::vector<FuncArg*> & args) const {
    WC_GUARD(mArgList);
    mArgList->getArgs(args);
}

const DataValue * Func::getArg(const char * argName) const {
    auto iter = mArgValues.find(argName);
    WC_GUARD(iter != mArgValues.end(), nullptr);
    return &iter->second;
}

bool Func::codegen(CodegenCtx & cgCtx) {
    // TODO: verify all codepaths return a value if there is a return value
    
    // Register the function with the parent module.
    Module * module = firstParentOfType<Module>();
    WC_ASSERT(module);
    
    if (!module->registerFunc(*this)) {
        compileError("Duplicate function named '%s' defined!", name());
        return false;
    }
    
    // Get the list of function args
    std::vector<FuncArg*> funcArgs;
    
    if (mArgList) {
        funcArgs.reserve(mArgList->numArgs());
        mArgList->getArgs(funcArgs);
    }
    
    // Check for duplicate argument names
    WC_GUARD(compileCheckForDuplicateArgNames(funcArgs), false);
    
    // Get the llvm types for the function arguments
    std::vector<llvm::Type*> fnArgTypesLLVM;
    WC_GUARD(determineLLVMArgTypes(cgCtx, funcArgs, fnArgTypesLLVM), false);
    
    // Get the llvm type for the function return:
    llvm::Type * fnRetTy = mReturnType.dataType().llvmType(cgCtx);
    
    if (!fnRetTy) {
        compileError("Unable to determine the llvm type of return type '%s'!", mReturnType.dataType().name());
        return false;
    }
    
    // Create the function signature:
    // TODO: support varargs
    llvm::FunctionType * fnType = llvm::FunctionType::get(fnRetTy,
                                                          fnArgTypesLLVM,
                                                          false);
    WC_ASSERT(fnType);
    
    // Create the function object itself
    // TODO: support different linkage types
    mLLVMFunc = llvm::Function::Create(fnType,
                                       llvm::Function::ExternalLinkage,
                                       mIdentifier.name(),
                                       cgCtx.module.mLLVMModule.get());
    WC_ASSERT(mLLVMFunc);
    
    // Save a list of the function arguments for later lookup by variables
    auto & llvmArgList = mLLVMFunc->getArgumentList();
    
    for (auto & llvmArg : llvmArgList) {
        size_t argNum = llvmArg.getArgNo();
        WC_ASSERT(argNum < funcArgs.size());
        FuncArg * funcArg = funcArgs[argNum];
        mArgValues[funcArg->name()] = DataValue(&llvmArg, &funcArg->dataType(), false);
    }
    
    // Request deferred codegen for the function body. Will generate the body code for all functions
    // after the function definitions themselves have been parsed...
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegen(deferredCgCtx);
    });
    
    return true;    // All good!
}

bool Func::deferredCodegen(CodegenCtx & cgCtx) {
    // This should NOT be null by the time this is called!
    WC_ASSERT(mLLVMFunc);
    
    // Create the function entry block and set it as the insert point for ir builder
    llvm::BasicBlock * fnEntryBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "func_entry_block", mLLVMFunc);
    cgCtx.irBuilder.SetInsertPoint(fnEntryBlock);
    
    // Generate code for the function body
    WC_GUARD(mScope.codegen(cgCtx), false);
    
    // Create the implicit return statement if required.
    // This is only allowable for void returning functions, for other functions not returning void it is
    // a compile error not to return a valid value.
    if (!cgCtx.irBuilder.GetInsertBlock()->getTerminator()) {
        if (mReturnType.dataType().isVoid()) {
            cgCtx.irBuilder.CreateRetVoid();
        }
        else {
            compileError("Missing a return statement at end of non void function!");
            return false;
        }
    }
    
    return true;    // Success!
}

bool Func::compileCheckForDuplicateArgNames(const std::vector<FuncArg*> & funcArgs) const {
    // TODO: handle args without names
    std::set<std::string> argNames;
    
    for (FuncArg * arg : funcArgs) {
        WC_ASSERT(arg);
        std::string argName = arg->name();
        
        if (argNames.count(argName) > 0) {
            compileError("Duplicate argument named '%s' in function argument list!", argName.c_str());
            return false;
        }
        
        argNames.insert(argName);
    }
    
    return true;    // All good!
}

bool Func::determineLLVMArgTypes(CodegenCtx & cgCtx,
                                 const std::vector<FuncArg*> & funcArgs,
                                 std::vector<llvm::Type*> & outputArgTypes) const
{
    for (FuncArg * arg : funcArgs) {
        WC_ASSERT(arg);
        const DataType & argDataType = arg->dataType();
        
        // Only valid, sized args type are allowed:
        if (argDataType.isSized()) {
            llvm::Type * llvmType = argDataType.llvmType(cgCtx);
            
            if (!llvmType) {
                compileError("Failed to determine the llvm type for argument '%s'!", arg->name());
                return false;
            }
            
            outputArgTypes.push_back(llvmType);
        }
        else {
            compileError("Invalid data type specified for argument '%s'!", arg->name());
            return false;
        }
    }
    
    return true;    // All good!
}

WC_END_NAMESPACE
