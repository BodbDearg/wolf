#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Function;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class Func;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Holds information about a function */
class Function {
public:
    /* Creates the function definition */
    Function(const AST::Func & astNode);
    
    /* The AST node for the function definition */
    const AST::Func & mASTNode;
    
    /* The llvm object for the function. Set during codegen. */
    llvm::Function * mLLVMFunc;
    
private:
    WC_DISALLOW_COPY_AND_ASSIGN(Function)
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
