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
    
    llvm::LLVMContext & llvmCtx;
    llvm::IRBuilder<> & irBuilder;
    llvm::Module & module;
};

WC_END_NAMESPACE
