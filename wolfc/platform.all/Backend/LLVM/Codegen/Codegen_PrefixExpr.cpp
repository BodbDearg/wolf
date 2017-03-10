//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_PrefixExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/PrefixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrefixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PrefixExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenPlusUnaryOp(*this, astNode.mExpr, false).codegen();
}

void Codegen::visit(const AST::PrefixExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMinusUnaryOp(*this, astNode.mExpr, false).codegen();
}

void Codegen::visit(const AST::PrefixExprAddrOf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenAddrOfUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::PrefixExprPtrDeref & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    constexpr const bool kLoadExprResult = true;
    CodegenPtrDerefUnaryOp(*this, astNode.mExpr, kLoadExprResult).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
