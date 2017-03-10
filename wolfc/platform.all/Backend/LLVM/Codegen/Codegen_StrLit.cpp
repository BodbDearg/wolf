//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/StrLit.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::StrLit & astNode) {
    // Use the constant code generator to do this since it's just the same thing, but convert the
    // result over to a non constant value:
    astNode.accept(mConstCodegen);
    Constant constant = mCtx.popConstant();
    
    if (constant.isValid()) {
        mCtx.pushValue(Value(constant.mLLVMConst, constant.mCompiledType, false, &astNode));
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
