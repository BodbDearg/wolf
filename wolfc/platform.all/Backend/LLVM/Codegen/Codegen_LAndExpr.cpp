#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_LAndExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/LAndExpr.hpp"
#include "AST/Nodes/NotExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::LAndExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::LAndExprAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
