//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_ShiftExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/PrefixExpr.hpp"
#include "AST/Nodes/ShiftExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

void Codegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

void Codegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
