#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_ShiftExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/ShiftExpr.hpp"
#include "AST/Nodes/UnaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
