#include "Codegen.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

Codegen::Codegen(CodegenCtx & ctx, const char * moduleName) :
    mCtx(ctx),
    mModuleName(moduleName),
    mConstCodegen(ctx),
    mCodegenDataType(mConstCodegen.mCodegenDataType),
    mAddrCodegen(ctx, *this, mConstCodegen, mCodegenDataType)
{
    WC_EMPTY_FUNC_BODY();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
