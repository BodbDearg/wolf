#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/ShiftExpr.hpp"
#include "AST/Nodes/UnaryExpr.hpp"
#include "CodegenConstBinaryOp_ShiftExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
