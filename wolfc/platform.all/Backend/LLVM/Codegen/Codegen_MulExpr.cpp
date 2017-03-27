//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_MulExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/MulExpr.hpp"
#include "AST/Nodes/ShiftExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

void Codegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

void Codegen::visit(const AST::MulExprRem & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenRemBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

void Codegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, false).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
