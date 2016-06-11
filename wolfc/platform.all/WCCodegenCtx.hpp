#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Module;
    class LLVMContext;
}

WC_BEGIN_NAMESPACE

class IDeferredCodegenStmnt;

/* Struct holding the context for code generation */
struct CodegenCtx {
    CodegenCtx(llvm::LLVMContext & llvmCtxIn,
               llvm::IRBuilder<> & irBuilderIn,
               llvm::Module & moduleIn)
    :
        llvmCtx(llvmCtxIn),
        irBuilder(irBuilderIn),
        module(moduleIn)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* The llvm context. */
    llvm::LLVMContext & llvmCtx;
    
    /* The llvm IR builder object. Used for creating most instructions. */
    llvm::IRBuilder<> & irBuilder;
    
    /* Reference to the llvm module being compiled. */
    llvm::Module & module;
    
    /**
     * A stack of AST nodes that will have deferred code generation performed.
     * Statements that modify normal program flow such as 'break', 'next' and 'return'
     * need to have their code generation deferred because they need to refer to basic blocks
     * around them that may not exist at the time code generation is invoked.
     *
     * Deferred code generation is perfomed in LIFO (stack) order, with the last item in the 
     * list being generated first.
     */
    std::vector<IDeferredCodegenStmnt*> deferredCogenStmnts;
};

WC_END_NAMESPACE
