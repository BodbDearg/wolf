#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <functional>
    #include <list>
    #include <llvm/IR/IRBuilder.h>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class LLVMContext;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class Module;
}

/* Struct holding the context for code generation */
struct CodegenCtx {
    /**
     * Type for a deferred code generation lambda callback.
     * The parameter given is this code generation context and the return value is whether the code generated
     * successfully or not.
     */
    typedef std::function<bool (CodegenCtx & cgCtx)> DeferredCodegenCallback;
    
    /* Creates the codegen context from the given llvm objects */
    CodegenCtx(llvm::LLVMContext & llvmCtxIn,
               llvm::IRBuilder<> & irBuilderIn,
               AST::Module & moduleIn)
    :
        llvmCtx(llvmCtxIn),
        irBuilder(irBuilderIn),
        module(moduleIn)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* Push the current codegen basic block to the stack and save for later popping. */
    void pushInsertBlock();
    
    /**
     * Pop a previous insert block from the stack.
     * There MUST be an insert block on the stack, assumes this is the case (undefined behavior otherwise).
     */
    void popInsertBlock();
    
    /* The llvm context. */
    llvm::LLVMContext & llvmCtx;
    
    /* The llvm IR builder object. Used for creating most instructions. */
    llvm::IRBuilder<> & irBuilder;
    
    /* The AST node for the module. Also contains the llvm module object. */
    AST::Module & module;
    
    /* A stack of code insert blocks pushed/saved for later restoring. */
    std::vector<llvm::BasicBlock*> insertBlockStack;    // TODO: use the linear allocator for efficiency
    
    /**
     * A list of callbacks that will be called in order to perform deferred code generation.
     *
     * For certain parts of the code generation, we must break our work into stages and defer
     * parts of the code generation until later. For example statements that modify normal program 
     * flow such as 'break' and 'next' need to have their code generation deferred because they
     * need to refer to basic blocks around them that may not exist at the time code generation is invoked.
     * Similiarly so, we must define all functions in a module before doing code generation for the function
     * bodies themselves so that we don't have the C/C++ limitation of having to define or forward declare
     * a function within a module before it is used.
     *
     * Note that deferred generation callbacks may themselves generate further deferred code generation
     * requests, allowing for a multi-pass complimation pipeline.
     *
     * Deferred code generation is perfomed in FIFO (queue) order, with the first item in the
     * list being generated first.
     */
    std::list<DeferredCodegenCallback> deferredCodegenCallbacks;    // TODO: use the linear allocator for efficiency
};

WC_END_NAMESPACE
