#include "Codegen.hpp"

#include "AST/Nodes/WCBoolLit.hpp"
#include "AST/Nodes/WCPrimaryExpr.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimaryExprIntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprBoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprStrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprIdentifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprReadnum & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprTime & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprRandExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
