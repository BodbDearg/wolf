#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssertStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/BreakStmnt.hpp"
#include "AST/Nodes/IfStmnt.hpp"
#include "AST/Nodes/LoopStmnt.hpp"
#include "AST/Nodes/NextStmnt.hpp"
#include "AST/Nodes/NoOpStmnt.hpp"
#include "AST/Nodes/PrintStmnt.hpp"
#include "AST/Nodes/ReturnStmnt.hpp"
#include "AST/Nodes/ScopeStmnt.hpp"
#include "AST/Nodes/Stmnt.hpp"
#include "AST/Nodes/VarDecl.hpp"
#include "AST/Nodes/WhileStmnt.hpp"
#include "Assert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::StmntNoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntPrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntAssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mDecl.accept(*this);
}

void Codegen::visit(const AST::StmntIfStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mIfStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntWhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mWhileStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntLoopStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLoopStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mScopeStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntBreakStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mBreakStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntNextStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mNextStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntReturnStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mReturnStmnt.accept(*this);
}

void Codegen::visit(const AST::StmntAssignExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
    
    // Clear up all values on the stack after this is done
    mCtx.clearValues();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
