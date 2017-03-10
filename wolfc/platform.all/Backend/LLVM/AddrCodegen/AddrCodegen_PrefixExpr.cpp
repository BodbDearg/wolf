//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddrCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_PrefixExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/PrefixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::PrefixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PrefixExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "+", "plus");
}

void AddrCodegen::visit(const AST::PrefixExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "-", "minus");
}

void AddrCodegen::visit(const AST::PrefixExprAddrOf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "&", "address of");
}

void AddrCodegen::visit(const AST::PrefixExprPtrDeref & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    constexpr const bool kLoadExprResult = false;
    CodegenPtrDerefUnaryOp(mCodegen, astNode.mExpr, kLoadExprResult).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
