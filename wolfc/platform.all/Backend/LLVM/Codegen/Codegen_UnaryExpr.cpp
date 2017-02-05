#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_UnaryExpr.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/UnaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::UnaryExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenPlusUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMinusUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
