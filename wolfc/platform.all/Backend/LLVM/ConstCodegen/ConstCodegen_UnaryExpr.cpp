#include "ConstCodegen.hpp"

#include "../CodegenConstUnaryOp/CodegenConstUnaryOp_UnaryExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/UnaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::UnaryExprPrimary & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstPlusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstMinusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
