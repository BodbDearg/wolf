#include "WCFunc.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCFuncArg.hpp"
#include "WCFuncArgList.hpp"
#include "WCIDeferredCodegenStmnt.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
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
                           *scope,
                           *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           FuncArgList * argList,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mArgList(argList),
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

bool Func::codegen(CodegenCtx & cgCtx) {
    // TODO: check for duplicate function definitions
    
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
    
    // Create the function signature:
    // TODO: support return types
    // TODO: support varargs
    llvm::FunctionType * fnType = llvm::FunctionType::get(llvm::Type::getVoidTy(cgCtx.llvmCtx),
                                                          fnArgTypesLLVM,
                                                          false);
    
    // Create the function object itself
    // TODO: support different linkage types
    llvm::Function * fn = llvm::Function::Create(fnType,
                                                 llvm::Function::ExternalLinkage,
                                                 mIdentifier.name(),
                                                 &cgCtx.module);
    
    // Create the function entry block and set it as the insert point for ir builder
    llvm::BasicBlock * fnEntryBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "func_entry_block", fn);
    cgCtx.irBuilder.SetInsertPoint(fnEntryBlock);
    
    // Do basic forward code generation (most code is generated this way):
    if (!mScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Do deferred code generation for 'next', 'break' and 'return' statements.
    // Note: need to push and pop current insert block because this process may modify insert block:
    cgCtx.pushInsertBlock();
    
    while (!cgCtx.deferredCodegenStmnts.empty()) {
        IDeferredCodegenStmnt * stmnt = cgCtx.deferredCodegenStmnts.back();
        cgCtx.deferredCodegenStmnts.pop_back();
        
        if (!stmnt->deferredCodegenStmnt(cgCtx)) {
            return false;
        }
    }
    
    cgCtx.popInsertBlock();
    
    // Create the return from the function
    // TODO: Support returning values
    cgCtx.irBuilder.CreateRetVoid();
    return true;
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
