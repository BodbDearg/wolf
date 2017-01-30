#include "Codegen.hpp"

#include "AST/Nodes/WCArrayLit.hpp"
#include "AST/Nodes/WCBoolLit.hpp"
#include "AST/Nodes/WCIdentifier.hpp"
#include "AST/Nodes/WCIntLit.hpp"
#include "AST/Nodes/WCPrimaryExpr.hpp"
#include "AST/Nodes/WCRandExpr.hpp"
#include "AST/Nodes/WCReadnumExpr.hpp"
#include "AST/Nodes/WCStrLit.hpp"
#include "AST/Nodes/WCTimeExpr.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimaryExprIntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprBoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprStrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprIdentifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mIdent.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprReadnum & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprTime & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PrimaryExprRandExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
