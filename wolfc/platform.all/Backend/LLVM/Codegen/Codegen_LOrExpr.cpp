#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_LOrExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/LAndExpr.hpp"
#include "AST/Nodes/LOrExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::LOrExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::LOrExprOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
