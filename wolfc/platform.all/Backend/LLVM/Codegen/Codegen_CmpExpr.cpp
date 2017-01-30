#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_CmpExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCCmpExpr.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpEQBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpNEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpLTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpLEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpGTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpGEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
