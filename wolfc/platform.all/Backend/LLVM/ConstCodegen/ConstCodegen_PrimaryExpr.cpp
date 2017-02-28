//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/BoolLit.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "AST/Nodes/IntLit.hpp"
#include "AST/Nodes/PrimaryExpr.hpp"
#include "AST/Nodes/StrLit.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::PrimaryExprIntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void ConstCodegen::visit(const AST::PrimaryExprBoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void ConstCodegen::visit(const AST::PrimaryExprStrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void ConstCodegen::visit(const AST::PrimaryExprArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void ConstCodegen::visit(const AST::PrimaryExprIdentifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mIdent.accept(*this);
}

void ConstCodegen::visit(const AST::PrimaryExprReadnum & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprReadnum");
}

void ConstCodegen::visit(const AST::PrimaryExprTime & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprTime");
}

void ConstCodegen::visit(const AST::PrimaryExprRandExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprRandExpr");
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
