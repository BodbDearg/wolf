#include "Codegen.hpp"

#include "AST/Nodes/WCDeclDef.hpp"
#include "AST/Nodes/WCFunc.hpp"
#include "AST/Nodes/WCVarDecl.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::DeclDefFunc & astNode) {
    astNode.mFunc.accept(*this);
}

void Codegen::visit(const AST::DeclDefVarDecl & astNode) {
    astNode.mVarDecl.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
