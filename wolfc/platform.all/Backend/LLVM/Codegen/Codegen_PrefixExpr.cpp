#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_PrefixExpr.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/PrefixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrefixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PrefixExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenPlusUnaryOp(*this, astNode.mExpr, false).codegen();
}

void Codegen::visit(const AST::PrefixExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMinusUnaryOp(*this, astNode.mExpr, false).codegen();
}

void Codegen::visit(const AST::PrefixExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
