#pragma once

#include "AST/WCASTNodeVisitor.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenCtx;

/* Similar to the 'Codegen' class but for compile time evaluation. */
class ConstCodegen : public AST::ASTNodeVisitor {
public:
    ConstCodegen(CodegenCtx & ctx);
    
    /* ASTNode visitor functions */
    virtual void visit(const AST::AddExprAdd & node) override;
    virtual void visit(const AST::AddExprBOr & node) override;
    virtual void visit(const AST::AddExprBXor & node) override;
    virtual void visit(const AST::AddExprNoOp & node) override;
    virtual void visit(const AST::AddExprSub & node) override;
    virtual void visit(const AST::AddExprTwoOps & node) override;
    virtual void visit(const AST::ArrayLit & node) override;
    virtual void visit(const AST::ArrayLitExprs & node) override;
    virtual void visit(const AST::ArrayLitExprsMulti & node) override;
    virtual void visit(const AST::ArrayLitExprsSingle & node) override;
    virtual void visit(const AST::AssertStmnt & node) override;
    virtual void visit(const AST::AssignExprAssign & node) override;
    virtual void visit(const AST::AssignExprAssignAdd & node) override;
    virtual void visit(const AST::AssignExprAssignARShift & node) override;
    virtual void visit(const AST::AssignExprAssignBAnd & node) override;
    virtual void visit(const AST::AssignExprAssignBase & node) override;
    virtual void visit(const AST::AssignExprAssignBOr & node) override;
    virtual void visit(const AST::AssignExprAssignBXor & node) override;
    virtual void visit(const AST::AssignExprAssignDiv & node) override;
    virtual void visit(const AST::AssignExprAssignLRShift & node) override;
    virtual void visit(const AST::AssignExprAssignLShift & node) override;
    virtual void visit(const AST::AssignExprAssignMod & node) override;
    virtual void visit(const AST::AssignExprAssignMul & node) override;
    virtual void visit(const AST::AssignExprAssignSub & node) override;
    virtual void visit(const AST::AssignExprBinaryOpBase & node) override;
    virtual void visit(const AST::AssignExprNoAssign & node) override;
    virtual void visit(const AST::BoolLit & node) override;
    virtual void visit(const AST::BreakStmntNoCond & node) override;
    virtual void visit(const AST::BreakStmntWithCond & node) override;
    virtual void visit(const AST::CastExprCast & node) override;
    virtual void visit(const AST::CastExprNoCast & node) override;
    virtual void visit(const AST::CmpExprEQ & node) override;
    virtual void visit(const AST::CmpExprGE & node) override;
    virtual void visit(const AST::CmpExprGT & node) override;
    virtual void visit(const AST::CmpExprLE & node) override;
    virtual void visit(const AST::CmpExprLT & node) override;
    virtual void visit(const AST::CmpExprNE & node) override;
    virtual void visit(const AST::CmpExprNoOp & node) override;
    virtual void visit(const AST::CmpExprTwoOps & node) override;
    virtual void visit(const AST::DeclDefFunc & node) override;
    virtual void visit(const AST::DeclDefVarDecl & node) override;
    virtual void visit(const AST::Func & node) override;
    virtual void visit(const AST::FuncArg & node) override;
    virtual void visit(const AST::FuncArgListMulti & node) override;
    virtual void visit(const AST::FuncArgListSingle & node) override;
    virtual void visit(const AST::FuncCall & node) override;
    virtual void visit(const AST::FuncCallArgListMulti & node) override;
    virtual void visit(const AST::FuncCallArgListSingle & node) override;
    virtual void visit(const AST::Identifier & node) override;
    virtual void visit(const AST::IfStmntElse & node) override;
    virtual void visit(const AST::IfStmntElseIf & node) override;
    virtual void visit(const AST::IfStmntNoElse & node) override;
    virtual void visit(const AST::IntLit & node) override;
    virtual void visit(const AST::LAndExprAnd & node) override;
    virtual void visit(const AST::LAndExprNoOp & node) override;
    virtual void visit(const AST::LoopStmntNoCond & node) override;
    virtual void visit(const AST::LoopStmntWithCond & node) override;
    virtual void visit(const AST::LOrExprNoOp & node) override;
    virtual void visit(const AST::LOrExprOr & node) override;
    virtual void visit(const AST::Module & node) override;
    virtual void visit(const AST::MulExprBAnd & node) override;
    virtual void visit(const AST::MulExprDiv & node) override;
    virtual void visit(const AST::MulExprMod & node) override;
    virtual void visit(const AST::MulExprMul & node) override;
    virtual void visit(const AST::MulExprNoOp & node) override;
    virtual void visit(const AST::MulExprTwoOps & node) override;
    virtual void visit(const AST::NextStmntNoCond & node) override;
    virtual void visit(const AST::NextStmntWithCond & node) override;
    virtual void visit(const AST::NoOpStmnt & node) override;
    virtual void visit(const AST::NotExprBNot & node) override;
    virtual void visit(const AST::NotExprLNot & node) override;
    virtual void visit(const AST::NotExprNoOp & node) override;
    virtual void visit(const AST::PostfixExprArrayLookup & node) override;
    virtual void visit(const AST::PostfixExprDec & node) override;
    virtual void visit(const AST::PostfixExprFuncCall & node) override;
    virtual void visit(const AST::PostfixExprInc & node) override;
    virtual void visit(const AST::PostfixExprIncDecBase & node) override;
    virtual void visit(const AST::PostfixExprNoPostfix & node) override;
    virtual void visit(const AST::PrimaryExprArrayLit & node) override;
    virtual void visit(const AST::PrimaryExprBoolLit & node) override;
    virtual void visit(const AST::PrimaryExprIdentifier & node) override;
    virtual void visit(const AST::PrimaryExprIntLit & node) override;
    virtual void visit(const AST::PrimaryExprRandExpr & node) override;
    virtual void visit(const AST::PrimaryExprReadnum & node) override;
    virtual void visit(const AST::PrimaryExprStrLit & node) override;
    virtual void visit(const AST::PrimaryExprTime & node) override;
    virtual void visit(const AST::PrimitiveType & node) override;
    virtual void visit(const AST::PrintStmnt & node) override;
    virtual void visit(const AST::RandExprRand & node) override;
    virtual void visit(const AST::RandExprSRand & node) override;
    virtual void visit(const AST::ReadnumExpr & node) override;
    virtual void visit(const AST::ReturnStmntNoCondVoid & node) override;
    virtual void visit(const AST::ReturnStmntNoCondWithValue & node) override;
    virtual void visit(const AST::ReturnStmntWithCondAndValue & node) override;
    virtual void visit(const AST::ReturnStmntWithCondBase & node) override;
    virtual void visit(const AST::ReturnStmntWithCondVoid & node) override;
    virtual void visit(const AST::Scope & node) override;
    virtual void visit(const AST::ScopeStmnt & node) override;
    virtual void visit(const AST::ShiftExprARShift & node) override;
    virtual void visit(const AST::ShiftExprLRShift & node) override;
    virtual void visit(const AST::ShiftExprLShift & node) override;
    virtual void visit(const AST::ShiftExprNoOp & node) override;
    virtual void visit(const AST::ShiftExprTwoOps & node) override;
    virtual void visit(const AST::StmntAssertStmnt & node) override;
    virtual void visit(const AST::StmntAssignExpr & node) override;
    virtual void visit(const AST::StmntBreakStmnt & node) override;
    virtual void visit(const AST::StmntIfStmnt & node) override;
    virtual void visit(const AST::StmntLoopStmnt & node) override;
    virtual void visit(const AST::StmntNextStmnt & node) override;
    virtual void visit(const AST::StmntNoOpStmnt & node) override;
    virtual void visit(const AST::StmntPrintStmnt & node) override;
    virtual void visit(const AST::StmntReturnStmnt & node) override;
    virtual void visit(const AST::StmntScopeStmnt & node) override;
    virtual void visit(const AST::StmntVarDecl & node) override;
    virtual void visit(const AST::StmntWhileStmnt & node) override;
    virtual void visit(const AST::StrLit & node) override;
    virtual void visit(const AST::TernaryExprNoCond & node) override;
    virtual void visit(const AST::TernaryExprWithCond & node) override;
    virtual void visit(const AST::TimeExpr & node) override;
    virtual void visit(const AST::TypeArray & node) override;
    virtual void visit(const AST::TypePrimitive & node) override;
    virtual void visit(const AST::UnaryExprMinus & node) override;
    virtual void visit(const AST::UnaryExprParen & node) override;
    virtual void visit(const AST::UnaryExprPlus & node) override;
    virtual void visit(const AST::UnaryExprPlusMinusBase & node) override;
    virtual void visit(const AST::UnaryExprPrimary & node) override;
    virtual void visit(const AST::VarDeclExplicitType & node) override;
    virtual void visit(const AST::VarDeclInferType & node) override;
    virtual void visit(const AST::WhileStmnt & node) override;
    
private:
    CodegenCtx & mCtx;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
