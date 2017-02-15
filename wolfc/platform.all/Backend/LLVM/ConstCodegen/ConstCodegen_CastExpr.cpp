#include "ConstCodegen.hpp"

#include "../CodegenConstUnaryOp/CodegenConstUnaryOp_CastExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PrimaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::CastExprNoCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::CastExprCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCastUnaryOp(*this, astNode.mExpr, astNode.mType).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
