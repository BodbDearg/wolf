#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::PostfixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprDec");
}

void ConstCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprFuncCall");
}

void ConstCodegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprInc");
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
