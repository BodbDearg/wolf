#include "Codegen.hpp"

#include "AST/Nodes/WCFunc.hpp"
#include "AST/Nodes/WCFuncArg.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "Function.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

static void compileCheckForDuplicateArgNames(CodegenCtx & ctx, const std::vector<AST::FuncArg*> & funcArgs) {
    // TODO: handle args without names
    std::set<std::string> argNames;
    
    for (const AST::FuncArg * arg : funcArgs) {
        WC_ASSERT(arg);
        std::string argName = arg->name();
        
        if (argNames.count(argName) > 0) {
            ctx.error("Duplicate argument named '%s' in function argument list!", argName.c_str());
        }
    }
}

void Codegen::visit(const AST::Func & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Firstly register the function with the module.
    // If this function is a duplicate then a compile error will occur.
    mCtx.registerModuleFunc(astNode);
    
    // Get the function object in the module.
    // It should now exist, but if it's not found then bail out...
    Function * func = mCtx.getModuleFunc(astNode.name());
    WC_GUARD(func);
    
    // Codegen all the function arguments and save their llvm types
    const auto & astFuncArgs = astNode.getArgs();
    std::vector<llvm::Type*> funcArgLLVMTypes;
    funcArgLLVMTypes.reserve(astFuncArgs.size());
    
    for (const AST::FuncArg * astArg : astFuncArgs) {
        // Visit the arg, which will codegen it's type and push it onto the type stack.
        // If the type was not generated okay, just substitute with 'int64' so we can
        // roughly get it right and continue:
        WC_ASSERT(astArg);
        astArg->accept(*this);
        llvm::Type * llvmArgType = mCtx.popLLVMType();
        
        // Fallback to 'int64' just to get the right arg count, if codegenning the arg fails
        if (!llvmArgType) {
            llvmArgType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
            WC_ASSERT(llvmArgType);
        }
        
        funcArgLLVMTypes.push_back(llvmArgType);
    }
    
    // Compile check for duplicate argument names
    compileCheckForDuplicateArgNames(mCtx, astFuncArgs);
    
    // Get the return data type for the function and codegen the llvm type.
    // If we fail to codegen the llvm type, just use 'int64' as a placeholder.
    DataType & returnDataType = astNode.returnDataType();
    returnDataType.accept(mCodegenDataType);
    llvm::Type * llvmReturnType = mCtx.popLLVMType();
    
    if (!llvmReturnType) {
        llvmReturnType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    }
    
    // Create the function signature:
    // TODO: support varargs
    llvm::FunctionType * llvmFnType = llvm::FunctionType::get(llvmReturnType,
                                                              funcArgLLVMTypes,
                                                              false);
    
    WC_ASSERT(llvmFnType);
    
    // Create the function object itself
    // TODO: support different linkage types
    WC_ASSERT(mCtx.mLLVMModule);
    func->mLLVMFunc = llvm::Function::Create(llvmFnType,
                                             llvm::Function::ExternalLinkage,
                                             astNode.name(),
                                             mCtx.mLLVMModule.get());
    
    WC_ASSERT(func->mLLVMFunc);
    
    #warning FIXME: FUNC codegen
    /*
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
