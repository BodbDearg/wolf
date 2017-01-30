#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PostfixExprNoPostfix & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
