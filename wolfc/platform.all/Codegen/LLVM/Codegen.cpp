#include "Codegen.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

Codegen::Codegen(CodegenCtx & ctx, const char * moduleName) :
    mCtx(ctx),
    mModuleName(moduleName),
    mConstCodegen(ctx),
    mCodegenDataType(mCtx, mConstCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

void Codegen::visit(const AST::AddExprAdd & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBOr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprBXor & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprSub & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AddExprTwoOps & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprs & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsMulti & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsSingle & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssertStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssign & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignAdd & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignARShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBAnd & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBase & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBOr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignBXor & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignDiv & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignLRShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignLShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignMod & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignMul & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprAssignSub & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprBinaryOpBase & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssignExprNoAssign & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BoolLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntNoCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntWithCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CastExprCast & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CastExprNoCast & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprEQ & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprGE & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprGT & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprLE & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprLT & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprNE & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::CmpExprTwoOps & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArg & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArgListMulti & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncArgListSingle & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCall & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListMulti & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListSingle & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::Identifier & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElse & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElseIf & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntNoElse & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IntLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LAndExprAnd & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LAndExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntNoCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntWithCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LOrExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LOrExprOr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprBAnd & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprDiv & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprMod & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprMul & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::MulExprTwoOps & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntNoCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntWithCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NoOpStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprBNot & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprLNot & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NotExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprArrayLookup & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprDec & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprFuncCall & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprInc & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprIncDecBase & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprNoPostfix & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprArrayLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprBoolLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprIdentifier & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprIntLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprRandExpr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprReadnum & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprStrLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimaryExprTime & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrimitiveType & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PrintStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprRand & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprSRand & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReadnumExpr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondWithValue & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondBase & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::Scope & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ScopeStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprARShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprLRShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprLShift & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprNoOp & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ShiftExprTwoOps & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntAssertStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntAssignExpr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntBreakStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntIfStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntLoopStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntNextStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntNoOpStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntPrintStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntReturnStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntScopeStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntVarDecl & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StmntWhileStmnt & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StrLit & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TernaryExprNoCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TernaryExprWithCond & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TimeExpr & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TypeArray & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TypePrimitive & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprMinus & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprParen & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPlus & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPlusMinusBase & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::UnaryExprPrimary & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclExplicitType & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclInferType & astNode) {
     #warning TODO: Codegen this node
}

void Codegen::visit(const AST::WhileStmnt & astNode) {
     #warning TODO: Codegen this node
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
