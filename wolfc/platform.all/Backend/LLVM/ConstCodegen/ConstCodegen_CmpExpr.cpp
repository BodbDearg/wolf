#include "ConstCodegen.hpp"

#include "../CodegenConstBinaryOp/CodegenConstBinaryOp_CmpExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AddExpr.hpp"
#include "AST/Nodes/CmpExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpEQBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpNEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpLTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpLEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpGTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpGEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
