#include "Codegen.hpp"

#include "AST/Nodes/WCFunc.hpp"
#include "CodegenCtx.hpp"
#include "Function.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::Func & node) {
    // Firstly register the function with the module.
    // If this function is a duplicate then a compile error will occur.
    mCtx.registerModuleFunc(node);
    
    // Get the function object in the module.
    // It should now exist, but if it's not found then bail out...
    Function * func = mCtx.getModuleFunc(node.name());
    WC_GUARD(func);
    
    #warning FIXME: FUNC codegen
    /*
    // Get the list of function args
    std::vector<FuncArg*> funcArgs;
    
    if (mArgList) {
        funcArgs.reserve(mArgList->numArgs());
        mArgList->getArgs(funcArgs);
    }
    
    // Check for duplicate argument names
    WC_GUARD(compileCheckForDuplicateArgNames(funcArgs), false);
    
    // Generate the code for each arg (llvm types):
    for (FuncArg * arg : funcArgs) {
        WC_GUARD(arg->codegen(cgCtx, *this), false);
    }
    
    // Get the llvm types for the function arguments
    std::vector<llvm::Type*> fnArgTypesLLVM;
    WC_GUARD(getLLVMArgTypes(funcArgs, fnArgTypesLLVM), false);
    
    // Generate the llvm type for the function return:
    llvm::Type * fnRetType = codegenLLVMReturnType(cgCtx);
    WC_GUARD(fnRetType, false);
    */
    
    llvm::FunctionType * llvmFnType = llvm::FunctionType::get(llvm::Type::getVoidTy(mCtx.mLLVMCtx),
                                                          {},
                                                          false);
    
    WC_ASSERT(mCtx.mLLVMModule);
    func->mLLVMFunc = llvm::Function::Create(llvmFnType,
                                             llvm::Function::ExternalLinkage,
                                             node.name(),
                                             mCtx.mLLVMModule.get());
    
    WC_ASSERT(func->mLLVMFunc);
    #warning FIXME: FUNC codegen
    /*
    // Create the function signature:
    // TODO: support varargs
    llvm::FunctionType * fnType = llvm::FunctionType::get(fnRetType,
                                                          fnArgTypesLLVM,
                                                          false);
    WC_ASSERT(fnType);
    
    // Create the function object itself
    // TODO: support different linkage types
    mLLVMFunc = llvm::Function::Create(fnType,
                                       llvm::Function::ExternalLinkage,
                                       mIdentifier.name(),
                                       cgCtx.module.getLLVMModule());
    WC_ASSERT(mLLVMFunc);
    
    // Save a list of the function arguments for later lookup by variables
    auto & llvmArgList = mLLVMFunc->getArgumentList();
    
    for (auto & llvmArg : llvmArgList) {
        size_t argNum = llvmArg.getArgNo();
        WC_ASSERT(argNum < funcArgs.size());
        FuncArg * funcArg = funcArgs[argNum];
        WC_ASSERT(funcArg);
        DataType & argDataType = funcArg->dataType();
        bool requiresLoad = argDataType.requiresStorage();
        
        mArgValues[funcArg->name()] = DataValue(funcArg,
                                                &llvmArg,
                                                &argDataType,
                                                requiresLoad);
    }
    
    // Request deferred codegen for the function body. Will generate the body code for all functions
    // after the function definitions themselves have been parsed...
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegen(deferredCgCtx);
    });
    */
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
