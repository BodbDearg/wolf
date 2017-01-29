#include "ConstCodegen.hpp"

#include "AST/Nodes/WCCmpExpr.hpp"
#include "AST/Nodes/WCNotExpr.hpp"
#include "CodegenConstUnaryOp/CodegenConstUnaryOp_NotExpr.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLNotUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBNotUnaryOp(*this, astNode.mExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
