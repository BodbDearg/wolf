#include "Codegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
