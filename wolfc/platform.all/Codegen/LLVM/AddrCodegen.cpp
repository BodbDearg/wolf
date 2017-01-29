#include "AddrCodegen.hpp"

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
#include "AST/Nodes/WCIExpr.hpp"
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
#include "Codegen.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

AddrCodegen::AddrCodegen(CodegenCtx & ctx,
                         Codegen & codegen,
                         ConstCodegen & constCodegen,
                         CodegenDataType & codegenDataType)
:
    mCtx(ctx),
    mCodegen(codegen),
    mConstCodegen(constCodegen),
    mCodegenDataType(codegenDataType)
{
    WC_EMPTY_FUNC_BODY();
}

void AddrCodegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "+",
                                   "add");
}

void AddrCodegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "|",
                                   "bitwise or");
}

void AddrCodegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "^",
                                   "bitwise xor");
}

void AddrCodegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "-",
                                   "subtract");
}

void AddrCodegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void AddrCodegen::visit(const AST::ArrayLitExprs & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ArrayLitExprs");
}

void AddrCodegen::visit(const AST::ArrayLitExprsMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ArrayLitExprsMulti");
}

void AddrCodegen::visit(const AST::ArrayLitExprsSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ArrayLitExprsSingle");
}

void AddrCodegen::visit(const AST::AssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "AssertStmnt");
}

void AddrCodegen::visit(const AST::AssignExprAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprAssignSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of an assign expression!");
}

void AddrCodegen::visit(const AST::AssignExprNoAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a bool literal!");
}

void AddrCodegen::visit(const AST::BreakStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntNoCond");
}

void AddrCodegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "BreakStmntWithCond");
}

void AddrCodegen::visit(const AST::CastExprCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get the data type being casted from and to
    astNode.mExpr.dataType().accept(mCodegenDataType);
    CompiledDataType fromType = mCtx.popCompiledDataType();
    astNode.mType.dataType().accept(mCodegenDataType);
    CompiledDataType toType = mCtx.popCompiledDataType();
    
    // Issue the error
    mCtx.error(astNode,
               "Can't take the address of a cast operation casting from "
               "type '%s' to type '%s'!",
               fromType.getDataType().name().c_str(),
               toType.getDataType().name().c_str());
}

void AddrCodegen::visit(const AST::CastExprNoCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "==",
                                   "equals");
}

void AddrCodegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   ">=",
                                   "greater than or equal to");
}

void AddrCodegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   ">",
                                   "greater than");
}

void AddrCodegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "<=",
                                   "less than or equal to");
}

void AddrCodegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "<",
                                   "less than");
}

void AddrCodegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "!=",
                                   "not equals");
}

void AddrCodegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::DeclDefFunc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "DeclDefFunc");
}

void AddrCodegen::visit(const AST::DeclDefVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "DeclDefVarDecl");
}

void AddrCodegen::visit(const AST::Func & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Func");
}

void AddrCodegen::visit(const AST::FuncArg & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncArg");
}

void AddrCodegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void AddrCodegen::visit(const AST::FuncCallArgListMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncCallArgListMulti");
}

void AddrCodegen::visit(const AST::FuncCallArgListSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "FuncCallArgListSingle");
}

void AddrCodegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void AddrCodegen::visit(const AST::IfStmntElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntElse");
}

void AddrCodegen::visit(const AST::IfStmntElseIf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntElseIf");
}

void AddrCodegen::visit(const AST::IfStmntNoElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "IfStmntNoElse");
}

void AddrCodegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a integer literal!");
}

void AddrCodegen::visit(const AST::LAndExprAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "and",
                                   "logical and");
}

void AddrCodegen::visit(const AST::LAndExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::LoopStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "LoopStmntNoCond");
}

void AddrCodegen::visit(const AST::LoopStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "LoopStmntWithCond");
}

void AddrCodegen::visit(const AST::LOrExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::LOrExprOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "or",
                                   "logical or");
}

void AddrCodegen::visit(const AST::Module & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Module");
}

void AddrCodegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "&",
                                   "bitwise and");
}

void AddrCodegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "/",
                                   "divide");
}

void AddrCodegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "%",
                                   "modulus");
}

void AddrCodegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "*",
                                   "multiply");
}

void AddrCodegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::NextStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NextStmntNoCond");
}

void AddrCodegen::visit(const AST::NextStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NextStmntWithCond");
}

void AddrCodegen::visit(const AST::NoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "NoOpStmnt");
}

void AddrCodegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "~", "bitwise not");
}

void AddrCodegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "not", "logical not");
}

void AddrCodegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void AddrCodegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void AddrCodegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprNoPostfix & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprBoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprIdentifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mIdent.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprIntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprRandExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprReadnum & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprStrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mLit.accept(*this);
}

void AddrCodegen::visit(const AST::PrimaryExprTime & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PrimitiveType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrimitiveType");
}

void AddrCodegen::visit(const AST::PrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PrintStmnt");
}

void AddrCodegen::visit(const AST::RandExprRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a rand() expression!");
}

void AddrCodegen::visit(const AST::RandExprSRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a srand() expression!");
}

void AddrCodegen::visit(const AST::ReadnumExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a readnum() expression!");
}

void AddrCodegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntNoCondVoid");
}

void AddrCodegen::visit(const AST::ReturnStmntNoCondWithValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntNoCondWithValue");
}

void AddrCodegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntWithCondAndValue");
}

void AddrCodegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ReturnStmntWithCondVoid");
}

void AddrCodegen::visit(const AST::Scope & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "Scope");
}

void AddrCodegen::visit(const AST::ScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "ScopeStmnt");
}

void AddrCodegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   ">>",
                                   "arithmetic right shift");
}

void AddrCodegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   ">>>",
                                   "logical right shift");
}

void AddrCodegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,
                                   astNode.mRightExpr,
                                   "<<",
                                   "left shift");
}

void AddrCodegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::StmntAssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntAssertStmnt");
}

void AddrCodegen::visit(const AST::StmntAssignExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::StmntBreakStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntBreakStmnt");
}

void AddrCodegen::visit(const AST::StmntIfStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntIfStmnt");
}

void AddrCodegen::visit(const AST::StmntLoopStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntLoopStmnt");
}

void AddrCodegen::visit(const AST::StmntNextStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntNextStmnt");
}

void AddrCodegen::visit(const AST::StmntNoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntNoOpStmnt");
}

void AddrCodegen::visit(const AST::StmntPrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntPrintStmnt");
}

void AddrCodegen::visit(const AST::StmntReturnStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntReturnStmnt");
}

void AddrCodegen::visit(const AST::StmntScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntScopeStmnt");
}

void AddrCodegen::visit(const AST::StmntVarDecl & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntVarDecl");
}

void AddrCodegen::visit(const AST::StmntWhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "StmntWhileStmnt");
}

void AddrCodegen::visit(const AST::StrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a string literal!");
}

void AddrCodegen::visit(const AST::TernaryExprNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::TernaryExprWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // FIXME: should we be able to do this?
    mCtx.error(astNode, "Can't take the address of a ternary expression!");
}

void AddrCodegen::visit(const AST::TimeExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Can't take the address of a time() expression!");
}

void AddrCodegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "TypeArray");
}

void AddrCodegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "TypePrimitive");
}

void AddrCodegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "-", "minus");
}

void AddrCodegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    cantTakeAddressOfUnaryOpError(astNode, "+", "plus");
}

void AddrCodegen::visit(const AST::UnaryExprPrimary & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::VarDeclExplicitType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "VarDeclExplicitType");
}

void AddrCodegen::visit(const AST::VarDeclInferType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "VarDeclInferType");
}

void AddrCodegen::visit(const AST::WhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "WhileStmnt");
}

void AddrCodegen::codegenNotSupportedForNodeTypeError(const AST::ASTNode & node,
                                                      const char * nodeClassName)
{
    mCtx.error(node,
               "Code generating the address of AST node type '%s' is not supported!",
               nodeClassName);
}

void AddrCodegen::cantTakeAddressOfUnaryOpError(const AST::ASTNode & exprNode,
                                                const char * opSymbol,
                                                const char * opName)
{
    // Get the node as an expression
    const AST::IExpr * expr = dynamic_cast<const AST::IExpr*>(&exprNode);
    WC_ASSERT(expr);
    
    // Get the parent of the node (the unary operator node)
    const AST::ASTNode * parent = exprNode.mParent;
    WC_ASSERT(parent);
    
    // Compile the data type
    expr->dataType().accept(mCodegenDataType);
    CompiledDataType type = mCtx.popCompiledDataType();
    
    // Issue the error
    mCtx.error(*parent,
               "Can't take the address of the result of unary operator '%s' (%s) for an "
               "expression of type '%s'!",
               opSymbol,
               opName,
               type.getDataType().name().c_str());
}

void AddrCodegen::cantTakeAddressOfBinaryOpError(const AST::ASTNode & leftExprNode,
                                                 const AST::ASTNode & rightExprNode,
                                                 const char * opSymbol,
                                                 const char * opName)
{
    // Get the left and right nodes as expressions
    const AST::IExpr * leftExpr = dynamic_cast<const AST::IExpr*>(&leftExprNode);
    const AST::IExpr * rightExpr = dynamic_cast<const AST::IExpr*>(&rightExprNode);
    WC_ASSERT(leftExpr);
    WC_ASSERT(rightExpr);
    
    // Get the parent of the left and right expression (the binary operator node)
    const AST::ASTNode * parent = leftExprNode.mParent;
    WC_ASSERT(parent);
    
    // Compile the left and right data types
    leftExpr->dataType().accept(mCodegenDataType);
    CompiledDataType leftType = mCtx.popCompiledDataType();
    rightExpr->dataType().accept(mCodegenDataType);
    CompiledDataType rightType = mCtx.popCompiledDataType();
    
    // Issue the error
    mCtx.error(*parent,
               "Can't take the address of the result of binary operator '%s' (%s) for a "
               "left expression of type '%s' and right expression of type '%s'!",
               opSymbol,
               opName,
               leftType.getDataType().name().c_str(),
               rightType.getDataType().name().c_str());
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
