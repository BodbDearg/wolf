#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenLazyLogicalBinaryOp.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/LAndExpr.hpp"
#include "AST/Nodes/NotExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::LAndExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::LAndExprAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLazyLogicalBinaryOp(*this,
                               astNode.mLeftExpr,
                               astNode.mRightExpr,
                               CodegenLazyLogicalBinaryOp::OpType::kAnd).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
