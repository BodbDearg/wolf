#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/FuncArg.hpp"
#include "AST/Nodes/Scope.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Primitives/FuncDataType.hpp"
#include "Finally.hpp"
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

/* Do deferred code generation for the body of the function */
static void doDeferredFuncCodegen(Codegen & cg, const AST::Func & astNode, Constant & function) {
    // Record the node being visited in this scope and the current function being visited
    CodegenCtxPushASTNode pushASTNode(astNode, cg.mCtx);
    SetCurrentCGFunction setCGFunction(cg.mCtx, function);
    
    // Get the llvm function
    WC_ASSERT(function.mLLVMConst);
    llvm::Function * llvmFn = llvm::cast<llvm::Function>(function.mLLVMConst);
    
    // Get the prologue BB for the function and make it the insert point
    llvm::BasicBlock & fnPrologueBB = llvmFn->getEntryBlock();
    cg.mCtx.mIRBuilder.SetInsertPoint(&fnPrologueBB);
    
    // Create the user function entry block and set it as the insert point for ir builder
    std::string fnUserEntryBBLbl = StringUtils::appendLineInfo("Func:UserEntryBB",
                                                               astNode.getScope().getStartToken());
    
    llvm::BasicBlock * fnUserEntryBB = llvm::BasicBlock::Create(cg.mCtx.mLLVMCtx,
                                                                fnUserEntryBBLbl,
                                                                llvmFn);
    
    WC_ASSERT(fnUserEntryBB);
    
    // Tie the prologue BB to the user entry BB and switch to inserting code in the user entry BB
    cg.mCtx.mIRBuilder.CreateBr(fnUserEntryBB);
    cg.mCtx.mIRBuilder.SetInsertPoint(fnUserEntryBB);
    
    // Get the data type for this function and ensure it is valid:
    cg.mCodegenDataType.visitASTNode(astNode);
    CompiledDataType funcCDT = cg.mCtx.popCompiledDataType();
    WC_GUARD(funcCDT.isValid());
    WC_GUARD(funcCDT.getDataType().getTypeId() == DataTypeId::kFunc);
    const FuncDataType & funcDataType = static_cast<const FuncDataType&>(funcCDT.getDataType());
    
    // Generate code for the function body
    const AST::Scope & scope = astNode.getScope();
    scope.accept(cg);
    
    // Create the implicit return statement if required.
    // This is only allowable for void returning functions, for other functions not returning void it is
    // a compile error not to return a valid value.
    if (!cg.mCtx.mIRBuilder.GetInsertBlock()->getTerminator()) {
        if (funcDataType.mReturnType.isVoid()) {
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
    mCodegenDataType.visitASTNode(astNode);
    CompiledDataType fnCompiledTy = mCtx.popCompiledDataType();
    
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
                                                            astNode,
                                                            false);
    
    // Create the function object itself:
    // TODO: support different linkage types.
    WC_ASSERT(mCtx.mLLVMModule);
    llvm::Function * llvmFn = llvm::Function::Create(llvmFnTy,
                                                     llvm::Function::ExternalLinkage,
                                                     constant.mName,
                                                     mCtx.mLLVMModule.get());
    
    constant.mLLVMConst = llvmFn;
    WC_ASSERT(constant.mLLVMConst);
    
    // Create the basic block to hold the function prologue and make it the current insert point
    std::string fnPrologueBBLbl = StringUtils::appendLineInfo("Func:PrologueBB", astNode.getStartToken());
    llvm::BasicBlock * fnPrologueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, fnPrologueBBLbl, llvmFn);
    WC_ASSERT(fnPrologueBB);
    mCtx.mIRBuilder.SetInsertPoint(fnPrologueBB);
    Finally clearInsertPointOnExit([&](){ mCtx.mIRBuilder.ClearInsertionPoint(); });
    
    // Also register the function as a variable in the global scope.
    // Note: skip duplicate name checks because they are already done above:
    mCtx.mModuleValHolder.createVal(mCtx,
                                    constant.mName,
                                    llvmFn,
                                    fnCompiledTy,
                                    false,
                                    astNode,
                                    true);
    
    // Get the val holder for the function scope, will register the function argument variables in this
    ValHolder & funcValHolder = mCtx.getScopeValHolder(astNode.getScope());
    
    // Register the function arguments for lookup by variables:
    {
        auto & llvmArgList = llvmFn->getArgumentList();
        auto & funcArgs = astNode.getArgs();
        
        for (auto & llvmArg : llvmArgList) {
            size_t argNum = llvmArg.getArgNo();
            
            if (argNum < funcArgs.size()) {
                // Okay, get the llvm value for this argument
                llvm::Value * llvmArgVal = &llvmArg;
                
                // Get the compiled type for this argument.
                AST::FuncArg * funcArg = funcArgs[argNum];
                funcArg->accept(*this);
                CompiledDataType funcArgCDT = mCtx.popCompiledDataType();
                
                // Can only continue if this is valid data type
                if (!funcArgCDT.isValid()) {
                    continue;
                }
                
                // Arg data type cannot be void:
                const DataType & funcArgDT = funcArgCDT.getDataType();
                
                if (!funcArgDT.isSized()) {
                    mCtx.error(*funcArg,
                               "Invalid unsized data type '%s' for function argument!",
                               funcArgDT.name().c_str());
                    
                    continue;
                }
                
                // Need to allocate stack space for the argument and assign the function param to the arg:
                llvm::Value * llvmArgStackVal = mCtx.mIRBuilder.CreateAlloca(funcArgCDT.getLLVMType(),
                                                                             nullptr,
                                                                             "Func:AllocArgOnStack");
                
                WC_ASSERT(llvmArgStackVal);
                WC_ASSERTED_OP(mCtx.mIRBuilder.CreateStore(llvmArgVal, llvmArgStackVal));
                
                // Register this variable in the current scope
                funcValHolder.createVal(mCtx,
                                        funcArg->name(),
                                        llvmArgStackVal,
                                        funcArgCDT,
                                        true,
                                        *funcArg,
                                        false);
            }
            else {
                mCtx.error(astNode,
                           "Error! Found more llvm function arguments than AST node function arguments "
                           "during code generation. This shouldn't happen!");
                
                break;
            }
        }
        
        if (funcArgs.size() > llvmArgList.size()) {
            mCtx.error(astNode,
                       "Error! Found more AST function arguments than llvm function arguments! "
                       "Did the llvm arguments codegen ok?");
        }
    }
    
    // Deferred code generation for the inner body of the function.
    // Called after the forward code declaration for the module has been done.
    mCtx.mDeferredCgCallbacks_Module.push_back([&](){
       doDeferredFuncCodegen(*this, astNode, constant);
    });
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
