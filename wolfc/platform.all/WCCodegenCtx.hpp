#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Module;
}

WC_BEGIN_NAMESPACE

/* Struct holding the context for code generation */
struct CodegenCtx {
    CodegenCtx(llvm::IRBuilder<> & irBuilder, llvm::Module & module) :
        irBuilder(irBuilder),
        module(module)
    {
        WC_EMPTY_FUNC_BODY()
    }
    
    llvm::IRBuilder<> & irBuilder;
    llvm::Module & module;
};

WC_END_NAMESPACE
