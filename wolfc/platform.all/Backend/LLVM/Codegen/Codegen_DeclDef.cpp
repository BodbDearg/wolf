#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/DeclDef.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/VarDecl.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::DeclDefFunc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mFunc.accept(*this);
}

void Codegen::visit(const AST::DeclDefVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mVarDecl.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
