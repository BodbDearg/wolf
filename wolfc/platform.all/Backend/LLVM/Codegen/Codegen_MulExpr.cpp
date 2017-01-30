#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_MulExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/MulExpr.hpp"
#include "AST/Nodes/ShiftExpr.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenModBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
