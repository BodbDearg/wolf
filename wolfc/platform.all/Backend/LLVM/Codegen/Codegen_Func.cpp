#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/FuncArg.hpp"
#include "AST/Nodes/Scope.hpp"
#include "DataType/DataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * RAII Helper to set the current function being code generated and restore it again to whatever it 
 * was previously when done.
 */
struct SetCurrentCGFunction {
public:
    inline SetCurrentCGFunction(CodegenCtx & ctx, Constant & function) :
        mCtx(ctx),
        mPrevFunc(ctx.mCurFunc)
    {
        mCtx.mCurFunc = &function;
    }
    
    inline ~SetCurrentCGFunction() {
        mCtx.mCurFunc = mPrevFunc;
    }
    
private:
    CodegenCtx &    mCtx;
    Constant *      mPrevFunc;
};

#warning TODO: Eventually this should dissapeer once we register function vars in the same way as locals
/* Check for duplicate arg names in the function and issue compile errors if there are dupes */
static void compileCheckForDuplicateArgNames(CodegenCtx & ctx, const std::vector<AST::FuncArg*> & funcArgs) {
    // TODO: handle args without names
    std::set<std::string> argNames;
    
    for (const AST::FuncArg * arg : funcArgs) {
        WC_ASSERT(arg);
        std::string argName = arg->name();
        
        if (argNames.count(argName) > 0) {
            ctx.error(*arg, "Duplicate argument named '%s' in function argument list!", argName.c_str());
        }
    }
}

/* Do deferred code generation for the body of the function */
static void doDeferredFuncCodegen(Codegen & cg, const AST::Func & astNode, Constant & function) {
    // Record the node being visited in this scope and the current function being visited
    CodegenCtxPushASTNode pushASTNode(astNode, cg.mCtx);
    SetCurrentCGFunction setCGFunction(cg.mCtx, function);
    
    // Get the llvm function
    WC_ASSERT(function.mLLVMConst);
    llvm::Function * llvmFn = llvm::cast<llvm::Function>(function.mLLVMConst);
    
    // Create the function entry block and set it as the insert point for ir builder
    std::string entryBBLabel = StringUtils::appendLineInfo("func_entry_bb",
                                                           astNode.mScope.getStartToken());
    
    llvm::BasicBlock * fnEntryBlock = llvm::BasicBlock::Create(cg.mCtx.mLLVMCtx,
                                                               entryBBLabel,
                                                               llvmFn);
    
    WC_ASSERT(fnEntryBlock);
    cg.mCtx.mIRBuilder.SetInsertPoint(fnEntryBlock);
    
    // Generate code for the function body
    AST::Scope & scope = astNode.mScope;
    scope.accept(cg);
    
    // Create the implicit return statement if required.
    // This is only allowable for void returning functions, for other functions not returning void it is
    // a compile error not to return a valid value.
    if (!cg.mCtx.mIRBuilder.GetInsertBlock()->getTerminator()) {
        if (astNode.dataType().mReturnType.isVoid()) {
            // Need an implicit return, make it:
            cg.mCtx.mIRBuilder.CreateRetVoid();
        }
        else {
            // Can't do an implicit return, make sure all codepaths return a value:
            if (!scope.allCodepathsHaveUncondRet()) {
                cg.mCtx.error(astNode, "Not all codepaths return a value in function which requires a return value!");
            }
            
            // If the end statement is unreachable then let llvm know.
            // All codepaths in the scope return a value so the last bit of code can never be reached.
            cg.mCtx.mIRBuilder.CreateUnreachable();
        }
    }
}

void Codegen::visit(const AST::Func & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the function:
    astNode.dataType().accept(mCodegenDataType);
    CompiledDataType fnCompiledTy = mCtx.popCompiledDataType();
    
    #warning TODO: Eventually this should dissapeer once we register function vars in the same way as locals
    // Compile check for duplicate argument names
    compileCheckForDuplicateArgNames(mCtx, astNode.getArgs());
    
    // Determine the llvm function type for the function.
    // Note: if the function data type does not have a valid llvm function type then we'll just generate a
    // void/void function type so we can at least try to codegen the rest of the function and discover more errors.
    llvm::FunctionType * llvmFnTy = llvm::cast_or_null<llvm::FunctionType>(fnCompiledTy.getLLVMType());
    
    if (!llvmFnTy) {
        // Haven't got a valid llvm function type so just use a default one for now
        llvmFnTy = llvm::FunctionType::get(llvm::Type::getVoidTy(mCtx.mLLVMCtx), {}, false);
    }
    
    // Register a constant for the function in the global scope.
    // TODO: Someday support local functions here.
    Constant & constant = mCtx.mModuleValHolder.createConst(mCtx,
                                                            astNode.name(),
                                                            nullptr,
                                                            fnCompiledTy,
                                                            astNode);
    
    // Create the function object itself:
    // TODO: support different linkage types.
    WC_ASSERT(mCtx.mLLVMModule);
    constant.mLLVMConst = llvm::Function::Create(llvmFnTy,
                                                 llvm::Function::ExternalLinkage,
                                                 constant.mName,
                                                 mCtx.mLLVMModule.get());
    
    WC_ASSERT(constant.mLLVMConst);
    
    // Make sure the non constant registration of this also has the function value too
    Value * value = mCtx.mModuleValHolder.getVal(constant.mName);
    WC_ASSERT(value);
    value->mLLVMVal = constant.mLLVMConst;
    
    #warning FIXME: FUNC codegen - save llvm arg values
    /*
    // Save a list of the function arguments for later lookup by variables
    auto & llvmArgList = mLLVMFunc->getArgumentList();
    
    for (auto & llvmArg : llvmArgList) {
        size_t argNum = llvmArg.getArgNo();
        WC_ASSERT(argNum < funcArgs.size());
        FuncArg * funcArg = funcArgs[argNum];
        WC_ASSERT(funcArg);
     
        #warning COMPILE DATA TYPE
        DataType & argDataType = funcArg->dataType();
        bool requiresLoad = argDataType.requiresStorage();
        
        mArgValues[funcArg->name()] = DataValue(funcArg,
                                                &llvmArg,
                                                &argDataType,
                                                requiresLoad);
    }
    */
    
    // Deferred code generation for the inner body of the function.
    // Called after the forward code declaration for the module has been done.
    mCtx.mDeferredCgCallbacks_Module.push_back([&](){
       doDeferredFuncCodegen(*this, astNode, constant);
    });
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
