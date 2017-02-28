//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

Codegen::Codegen(CodegenCtx & ctx) :
    mCtx(ctx),
    mConstCodegen(ctx),
    mCodegenDataType(mConstCodegen.mCodegenDataType),
    mAddrCodegen(ctx, *this, mConstCodegen, mCodegenDataType)
{
    WC_EMPTY_FUNC_BODY();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
