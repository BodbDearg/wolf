//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/BoolLit.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // This just uses the constant code generator, it's the same thing:
    astNode.accept(mConstCodegen);
    
    // Repackage the result as a non constant however:
    Constant constant = mCtx.popConstant();
    WC_GUARD(constant.isValid());
    mCtx.pushValue(Value(constant.mLLVMConst, constant.mCompiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
