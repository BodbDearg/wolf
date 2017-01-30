#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "../CodegenDataType/CodegenDataType.hpp"
#include "AST/Nodes/WCArrayLit.hpp"
#include "AST/Nodes/WCArrayLitExprs.hpp"
#include "AST/Nodes/WCAssertStmnt.hpp"
#include "AST/Nodes/WCBreakStmnt.hpp"
#include "AST/Nodes/WCDeclDef.hpp"
#include "AST/Nodes/WCFunc.hpp"
#include "AST/Nodes/WCFuncArg.hpp"
#include "AST/Nodes/WCFuncCall.hpp"
#include "AST/Nodes/WCFuncCallArgList.hpp"
#include "AST/Nodes/WCIdentifier.hpp"
#include "AST/Nodes/WCIfStmnt.hpp"
#include "AST/Nodes/WCLoopStmnt.hpp"
#include "AST/Nodes/WCModule.hpp"
#include "AST/Nodes/WCNextStmnt.hpp"
#include "AST/Nodes/WCNoOpStmnt.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "AST/Nodes/WCRandExpr.hpp"
#include "AST/Nodes/WCReadnumExpr.hpp"
#include "AST/Nodes/WCReturnStmnt.hpp"
#include "AST/Nodes/WCScope.hpp"
#include "AST/Nodes/WCScopeStmnt.hpp"
#include "AST/Nodes/WCStmnt.hpp"
#include "AST/Nodes/WCStrLit.hpp"
#include "AST/Nodes/WCTimeExpr.hpp"
#include "AST/Nodes/WCVarDecl.hpp"
#include "AST/Nodes/WCWhileStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

ConstCodegen::ConstCodegen(CodegenCtx & ctx) :
    mCtx(ctx),
    mCodegenDataType(ctx, *this)
{
    WC_EMPTY_FUNC_BODY();
}

void ConstCodegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprs & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprsMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprsSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssertStmnt");
}

void ConstCodegen::visit(const AST::BreakStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntNoCond");
}

void ConstCodegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntWithCond");
}

void ConstCodegen::visit(const AST::DeclDefFunc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "DeclDefFunc");
}

void ConstCodegen::visit(const AST::DeclDefVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "DeclDefVarDecl");
}

void ConstCodegen::visit(const AST::Func & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Func");
}

void ConstCodegen::visit(const AST::FuncArg & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncArg");
}

void ConstCodegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncCall");
}

void ConstCodegen::visit(const AST::FuncCallArgListMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncCallArgListMulti");
}

void ConstCodegen::visit(const AST::FuncCallArgListSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncCallArgListSingle");
}

void ConstCodegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::IfStmntElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntElse");
}

void ConstCodegen::visit(const AST::IfStmntElseIf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntElseIf");
}

void ConstCodegen::visit(const AST::IfStmntNoElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntNoElse");
}

void ConstCodegen::visit(const AST::LoopStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "LoopStmntNoCond");
}

void ConstCodegen::visit(const AST::LoopStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "LoopStmntWithCond");
}

void ConstCodegen::visit(const AST::Module & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Module");
}

void ConstCodegen::visit(const AST::NextStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NextStmntNoCond");
}

void ConstCodegen::visit(const AST::NextStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NextStmntWithCond");
}

void ConstCodegen::visit(const AST::NoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NoOpStmnt");
}

void ConstCodegen::visit(const AST::PrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrintStmnt");
}

void ConstCodegen::visit(const AST::RandExprRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "RandExprRand");
}

void ConstCodegen::visit(const AST::RandExprSRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "RandExprSRand");
}

void ConstCodegen::visit(const AST::ReadnumExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReadnumExpr");
}

void ConstCodegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntNoCondVoid");
}

void ConstCodegen::visit(const AST::ReturnStmntNoCondWithValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntNoCondWithValue");
}

void ConstCodegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntWithCondAndValue");
}

void ConstCodegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntWithCondVoid");
}

void ConstCodegen::visit(const AST::Scope & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Scope");
}

void ConstCodegen::visit(const AST::ScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ScopeStmnt");
}

void ConstCodegen::visit(const AST::StmntAssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntAssertStmnt");
}

void ConstCodegen::visit(const AST::StmntAssignExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntAssignExpr");
}

void ConstCodegen::visit(const AST::StmntBreakStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntBreakStmnt");
}

void ConstCodegen::visit(const AST::StmntIfStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntIfStmnt");
}

void ConstCodegen::visit(const AST::StmntLoopStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntLoopStmnt");
}

void ConstCodegen::visit(const AST::StmntNextStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntNextStmnt");
}

void ConstCodegen::visit(const AST::StmntNoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntNoOpStmnt");
}

void ConstCodegen::visit(const AST::StmntPrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntPrintStmnt");
}

void ConstCodegen::visit(const AST::StmntReturnStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntReturnStmnt");
}

void ConstCodegen::visit(const AST::StmntScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntScopeStmnt");
}

void ConstCodegen::visit(const AST::StmntVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntVarDecl");
}

void ConstCodegen::visit(const AST::StmntWhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntWhileStmnt");
}

void ConstCodegen::visit(const AST::StrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TimeExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "TimeExpr");
}

void ConstCodegen::visit(const AST::VarDeclExplicitType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "VarDeclExplicitType");
}

void ConstCodegen::visit(const AST::VarDeclInferType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "VarDeclInferType");
}

void ConstCodegen::visit(const AST::WhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "WhileStmnt");
}

void ConstCodegen::codegenNotSupportedForNodeTypeError(const AST::ASTNode & node,
                                                       const char * nodeClassName)
{
    mCtx.error(node,
               "Constant code generation not supported for a node of type '%s'!",
               nodeClassName);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
