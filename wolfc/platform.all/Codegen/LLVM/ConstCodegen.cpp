#include "ConstCodegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCArrayLit.hpp"
#include "AST/Nodes/WCArrayLitExprs.hpp"
#include "AST/Nodes/WCAssertStmnt.hpp"
#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCBoolLit.hpp"
#include "AST/Nodes/WCBreakStmnt.hpp"
#include "AST/Nodes/WCCastExpr.hpp"
#include "AST/Nodes/WCCmpExpr.hpp"
#include "AST/Nodes/WCDeclDef.hpp"
#include "AST/Nodes/WCFunc.hpp"
#include "AST/Nodes/WCFuncArg.hpp"
#include "AST/Nodes/WCFuncCall.hpp"
#include "AST/Nodes/WCFuncCallArgList.hpp"
#include "AST/Nodes/WCIBasicCodegenNode.hpp"
#include "AST/Nodes/WCIdentifier.hpp"
#include "AST/Nodes/WCIfStmnt.hpp"
#include "AST/Nodes/WCIntLit.hpp"
#include "AST/Nodes/WCLAndExpr.hpp"
#include "AST/Nodes/WCLoopStmnt.hpp"
#include "AST/Nodes/WCLOrExpr.hpp"
#include "AST/Nodes/WCModule.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "AST/Nodes/WCNextStmnt.hpp"
#include "AST/Nodes/WCNoOpStmnt.hpp"
#include "AST/Nodes/WCNotExpr.hpp"
#include "AST/Nodes/WCPostfixExpr.hpp"
#include "AST/Nodes/WCPrimaryExpr.hpp"
#include "AST/Nodes/WCPrimitiveType.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "AST/Nodes/WCRandExpr.hpp"
#include "AST/Nodes/WCReadnumExpr.hpp"
#include "AST/Nodes/WCReturnStmnt.hpp"
#include "AST/Nodes/WCScope.hpp"
#include "AST/Nodes/WCScopeStmnt.hpp"
#include "AST/Nodes/WCShiftExpr.hpp"
#include "AST/Nodes/WCStmnt.hpp"
#include "AST/Nodes/WCStrLit.hpp"
#include "AST/Nodes/WCTernaryExpr.hpp"
#include "AST/Nodes/WCTimeExpr.hpp"
#include "AST/Nodes/WCType.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "AST/Nodes/WCVarDecl.hpp"
#include "AST/Nodes/WCWhileStmnt.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

ConstCodegen::ConstCodegen(CodegenCtx & ctx) :
    mCtx(ctx),
    mCodegenDataType(ctx)
{
    WC_EMPTY_FUNC_BODY();
}

void ConstCodegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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

void ConstCodegen::visit(const AST::AssignExprAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssign");
}

void ConstCodegen::visit(const AST::AssignExprAssignAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignAdd");
}

void ConstCodegen::visit(const AST::AssignExprAssignARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignARShift");
}

void ConstCodegen::visit(const AST::AssignExprAssignBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignBAnd");
}

void ConstCodegen::visit(const AST::AssignExprAssignBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignBOr");
}

void ConstCodegen::visit(const AST::AssignExprAssignBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignBXor");
}

void ConstCodegen::visit(const AST::AssignExprAssignDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignDiv");
}

void ConstCodegen::visit(const AST::AssignExprAssignLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignLRShift");
}

void ConstCodegen::visit(const AST::AssignExprAssignLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignLShift");
}

void ConstCodegen::visit(const AST::AssignExprAssignMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignMod");
}

void ConstCodegen::visit(const AST::AssignExprAssignMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignMul");
}

void ConstCodegen::visit(const AST::AssignExprAssignSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssignExprAssignSub");
}

void ConstCodegen::visit(const AST::AssignExprNoAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::BreakStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntNoCond");
}

void ConstCodegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntWithCond");
}

void ConstCodegen::visit(const AST::CastExprCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CastExprNoCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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

void ConstCodegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LAndExprAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LAndExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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

void ConstCodegen::visit(const AST::LOrExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LOrExprOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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

void ConstCodegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprDec");
}

void ConstCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprFuncCall");
}

void ConstCodegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprInc");
}

void ConstCodegen::visit(const AST::PostfixExprNoPostfix & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprBoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprIdentifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprIntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprRandExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprRandExpr");
}

void ConstCodegen::visit(const AST::PrimaryExprReadnum & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprReadnum");
}

void ConstCodegen::visit(const AST::PrimaryExprStrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprTime & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimaryExprTime");
}

void ConstCodegen::visit(const AST::PrimitiveType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Generate the data type
    const DataType & dataType = astNode.dataType();
    dataType.accept(mCodegenDataType);
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

void ConstCodegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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

void ConstCodegen::visit(const AST::TernaryExprNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TernaryExprWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TimeExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "TimeExpr");
}

void ConstCodegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprPrimary & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
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
