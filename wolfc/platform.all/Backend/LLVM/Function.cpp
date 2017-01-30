#include "Function.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

Function::Function(const AST::Func & astNode) :
    mASTNode(astNode),
    mLLVMFunc(nullptr)
{
    WC_EMPTY_FUNC_BODY();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
