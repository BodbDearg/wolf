//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddrCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PrimaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::PrimaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
