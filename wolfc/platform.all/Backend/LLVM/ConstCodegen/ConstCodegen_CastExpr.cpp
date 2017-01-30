#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PrimaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::CastExprNoCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::CastExprCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
