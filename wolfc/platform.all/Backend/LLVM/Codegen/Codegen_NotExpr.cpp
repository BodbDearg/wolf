#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_NotExpr.hpp"
#include "AST/Nodes/CmpExpr.hpp"
#include "AST/Nodes/NotExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLNotUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBNotUnaryOp(*this, astNode.mExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
