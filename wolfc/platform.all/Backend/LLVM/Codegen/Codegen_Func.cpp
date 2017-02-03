#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../Function.hpp"
#include "Assert.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/FuncArg.hpp"
#include "AST/Nodes/Scope.hpp"
#include "DataType/DataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* RAII Helper to set the current function being code generated and clear it again when done */
struct SetCurrentCGFunction {
public:
    SetCurrentCGFunction(CodegenCtx & ctx, Function & function) : mCtx(ctx) {
        mCtx.mCurFunction = &function;
    }
    
    ~SetCurrentCGFunction() {
        mCtx.mCurFunction = nullptr;
    }
    
private:
    CodegenCtx & mCtx;
};

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

void Codegen::visit(const AST::Func & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    #warning TODO register this as a cosntant
    // Firstly register the function with the module.
    // If this function is a duplicate then a compile error will occur.
    mCtx.registerModuleFunc(astNode);
    
    // Get the function object in the module.
    // It should now exist, but if it's not found then bail out...
    Function * function = mCtx.getModuleFunc(astNode.name());
    WC_GUARD(function);
    
    // Set the current function being code generated in the context.
    SetCurrentCGFunction setCGFunction(mCtx, *function);
    
    // Codegen the data type for the function:
    astNode.dataType().accept(mCodegenDataType);
    CompiledDataType funcCompiledType = mCtx.popCompiledDataType();
    
    // Compile check for duplicate argument names
    compileCheckForDuplicateArgNames(mCtx, astNode.getArgs());
    
    // Create the function object itself
    // TODO: support different linkage types
    WC_ASSERT(mCtx.mLLVMModule);
    
    if (funcCompiledType.getLLVMType()) {
        llvm::FunctionType * fnLLVMType = llvm::cast<llvm::FunctionType>(funcCompiledType.getLLVMType());
        function->mLLVMFunc = llvm::Function::Create(fnLLVMType,
                                                     llvm::Function::ExternalLinkage,
                                                     astNode.name(),
                                                     mCtx.mLLVMModule.get());
        
        WC_ASSERT(function->mLLVMFunc);
    }
    
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
    mCtx.mDeferredCgCallbacks_Module.push_back([=](){
       doDeferredFunctionCodegen(astNode, *function);
    });
}

void Codegen::doDeferredFunctionCodegen(const AST::Func & astNode, Function & function) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Set the current function being code generated in the context.
    SetCurrentCGFunction setCGFunction(mCtx, function);
    
    // Create the function entry block and set it as the insert point for ir builder
    WC_ASSERT(function.mLLVMFunc);
    std::string entryBBLabel = StringUtils::appendLineInfo("func_entry_bb",
                                                           astNode.mScope.getStartToken());
    
    llvm::BasicBlock * fnEntryBlock = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                               entryBBLabel,
                                                               function.mLLVMFunc);
    
    WC_ASSERT(fnEntryBlock);
    mCtx.mIRBuilder.SetInsertPoint(fnEntryBlock);
    
    // Generate code for the function body
    AST::Scope & scope = astNode.mScope;
    scope.accept(*this);
    
    // Create the implicit return statement if required.
    // This is only allowable for void returning functions, for other functions not returning void it is
    // a compile error not to return a valid value.
    if (!mCtx.mIRBuilder.GetInsertBlock()->getTerminator()) {
        if (astNode.dataType().mReturnType.isVoid()) {
            // Need an implicit return, make it:
            mCtx.mIRBuilder.CreateRetVoid();
        }
        else {
            // Can't do an implicit return, make sure all codepaths return a value:
            if (!scope.allCodepathsHaveUncondRet()) {
                mCtx.error(astNode, "Not all codepaths return a value in function which requires a return value!");
            }
            
            // If the end statement is unreachable then let llvm know.
            // All codepaths in the scope return a value so the last bit of code can never be reached.
            mCtx.mIRBuilder.CreateUnreachable();
        }
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
