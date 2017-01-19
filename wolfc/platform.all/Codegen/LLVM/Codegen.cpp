#include "Codegen.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

Codegen::Codegen(CodegenCtx & ctx, const char * moduleName) :
    mCtx(ctx),
    mModuleName(moduleName)
{
    WC_EMPTY_FUNC_BODY();
}

void Codegen::visit(const AST::AddExprAdd & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBOr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBXor & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprSub & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprs & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsMulti & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsSingle & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssertStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssign & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignAdd & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignARShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBAnd & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBase & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBOr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBXor & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignDiv & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignLRShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignLShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignMod & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignMul & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignSub & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprBinaryOpBase & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprNoAssign & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BoolLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CastExprCast & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CastExprNoCast & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprEQ & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprGE & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprGT & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprLE & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprLT & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprNE & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArg & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArgListMulti & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArgListSingle & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCall & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListMulti & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListSingle & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::Identifier & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElse & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElseIf & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntNoElse & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IntLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LAndExprAnd & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LAndExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LOrExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LOrExprOr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprBAnd & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprDiv & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprMod & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprMul & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntNoCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntWithCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NoOpStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprBNot & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprLNot & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprArrayLookup & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprDec & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprFuncCall & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprInc & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprIncDecBase & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprNoPostfix & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprArrayLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprBoolLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprIdentifier & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprIntLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprRandExpr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprReadnum & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprStrLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprTime & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimitiveType & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrintStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprRand & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprSRand & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReadnumExpr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondVoid & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondWithValue & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondAndValue & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondBase & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondVoid & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::Scope & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ScopeStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprARShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprLRShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprLShift & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprNoOp & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprTwoOps & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntAssertStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntAssignExpr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntBreakStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntIfStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntLoopStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntNextStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntNoOpStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntPrintStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntReturnStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntScopeStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntVarDecl & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntWhileStmnt & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StrLit & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TernaryExprNoCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TernaryExprWithCond & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TimeExpr & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TypeArray & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TypePrimitive & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprMinus & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprParen & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPlus & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPlusMinusBase & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPrimary & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclExplicitType & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclInferType & node) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::WhileStmnt & node) {
     #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
