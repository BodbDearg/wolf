#include "ConstCodegen.hpp"

#include "../CodegenConstBinaryOp/CodegenConstBinaryOp_MulExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "AST/Nodes/WCShiftExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstModBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
