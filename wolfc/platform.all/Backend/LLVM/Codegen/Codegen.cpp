#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/ArrayLitExprs.hpp"

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

void Codegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprs & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
