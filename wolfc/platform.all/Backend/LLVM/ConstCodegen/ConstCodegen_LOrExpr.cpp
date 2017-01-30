#include "ConstCodegen.hpp"

#include "../CodegenConstBinaryOp/CodegenConstBinaryOp_LOrExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/WCLAndExpr.hpp"
#include "AST/Nodes/WCLOrExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::LOrExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::LOrExprOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
