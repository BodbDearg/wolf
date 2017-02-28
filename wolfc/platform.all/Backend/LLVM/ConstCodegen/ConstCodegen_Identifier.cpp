//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/Identifier.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Grab the constant value
    const Constant * constant = mCtx.lookupConstantByName(astNode.name());
    
    if (!constant) {
        // Couldn't find any value with this name, issue a compile error:
        mCtx.error(astNode,
                   "No symbol named '%s' (variable, function etc.) in the current scope! "
                   "Can't lookup the constant value of the symbol!",
                   astNode.name());
        
        // Can't do anything else, bail out:
        return;
    }
    
    // Push it into the context
    mCtx.pushConstant(Constant(constant->mLLVMConst, constant->mCompiledType, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
