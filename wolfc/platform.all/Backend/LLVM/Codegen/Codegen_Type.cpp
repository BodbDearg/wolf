//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/Type.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCodegenDataType.visitASTNode(astNode);
}

void Codegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mType.accept(*this);
}

void Codegen::visit(const AST::TypePtr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCodegenDataType.visitASTNode(astNode);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
