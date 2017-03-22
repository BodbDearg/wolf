//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenConstUnaryOp/CodegenConstUnaryOp_PrefixExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/PrefixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::PrefixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::PrefixExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstPlusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::PrefixExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstMinusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::PrefixExprPtrDenull & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstPtrDenullUnaryOp(*this, astNode.mExpr).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
