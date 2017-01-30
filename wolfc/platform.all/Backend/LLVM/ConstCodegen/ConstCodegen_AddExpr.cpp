#include "ConstCodegen.hpp"

#include "../CodegenConstBinaryOp/CodegenConstBinaryOp_AddExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AddExpr.hpp"
#include "AST/Nodes/MulExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstAddBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstSubBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBXOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
