#include "ConstCodegen.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

ConstCodegen::ConstCodegen(CodegenCtx & ctx) :
    mCtx(ctx)
{
    WC_EMPTY_FUNC_BODY();
}

void ConstCodegen::visit(const AST::AddExprAdd & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprBOr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprBXor & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprSub & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AddExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprs & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprsMulti & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ArrayLitExprsSingle & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssertStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssign & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignAdd & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignARShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignBAnd & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignBase & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignBOr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignBXor & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignDiv & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignLRShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignLShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignMod & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignMul & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprAssignSub & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprBinaryOpBase & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::AssignExprNoAssign & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::BoolLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::BreakStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::BreakStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CastExprCast & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CastExprNoCast & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprEQ & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprGE & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprGT & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprLE & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprLT & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprNE & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::CmpExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::DeclDefFunc & node) {
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::DeclDefVarDecl & node) {
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::Func & node) {
    #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncArg & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncArgListMulti & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncArgListSingle & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncCall & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncCallArgListMulti & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::FuncCallArgListSingle & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::Identifier & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::IfStmntElse & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::IfStmntElseIf & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::IfStmntNoElse & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::IntLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LAndExprAnd & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LAndExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LoopStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LoopStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::Module & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LOrExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::LOrExprOr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprBAnd & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprDiv & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprMod & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprMul & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::MulExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NextStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NextStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NoOpStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NotExprBNot & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NotExprLNot & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::NotExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprArrayLookup & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprDec & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprFuncCall & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprInc & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprIncDecBase & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PostfixExprNoPostfix & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprArrayLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprBoolLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprIdentifier & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprIntLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprRandExpr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprReadnum & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprStrLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimaryExprTime & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrimitiveType & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::PrintStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::RandExprRand & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::RandExprSRand & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReadnumExpr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReturnStmntNoCondVoid & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReturnStmntNoCondWithValue & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReturnStmntWithCondAndValue & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReturnStmntWithCondBase & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ReturnStmntWithCondVoid & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::Scope & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ScopeStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprARShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprLRShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprLShift & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprNoOp & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::ShiftExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntAssertStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntAssignExpr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntBreakStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntIfStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntLoopStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntNextStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntNoOpStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntPrintStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntReturnStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntScopeStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntVarDecl & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StmntWhileStmnt & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::StrLit & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TernaryExprNoCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TernaryExprWithCond & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TimeExpr & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TypeArray & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::TypePrimitive & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprMinus & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprParen & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprPlus & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprPlusMinusBase & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::UnaryExprPrimary & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::VarDeclExplicitType & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::VarDeclInferType & node) {
     #warning TODO: Codegen this node
}

void ConstCodegen::visit(const AST::WhileStmnt & node) {
     #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
