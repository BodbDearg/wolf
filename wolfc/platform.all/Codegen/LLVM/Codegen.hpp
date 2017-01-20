#pragma once

#include "AST/WCASTNodeVisitor.hpp"
#include "CodegenDataType.hpp"
#include "ConstCodegen.hpp"
#include "WCFinally.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenCtx;

/* Generates code for the LLVM backend */
class Codegen : public AST::ASTNodeVisitor {
public:
    Codegen(CodegenCtx & ctx, const char * moduleName);
    
    /* ASTNode visitor functions */
    virtual void visit(const AST::AddExprAdd & astNode) override;
    virtual void visit(const AST::AddExprBOr & astNode) override;
    virtual void visit(const AST::AddExprBXor & astNode) override;
    virtual void visit(const AST::AddExprNoOp & astNode) override;
    virtual void visit(const AST::AddExprSub & astNode) override;
    virtual void visit(const AST::AddExprTwoOps & astNode) override;
    virtual void visit(const AST::ArrayLit & astNode) override;
    virtual void visit(const AST::ArrayLitExprs & astNode) override;
    virtual void visit(const AST::ArrayLitExprsMulti & astNode) override;
    virtual void visit(const AST::ArrayLitExprsSingle & astNode) override;
    virtual void visit(const AST::AssertStmnt & astNode) override;
    virtual void visit(const AST::AssignExprAssign & astNode) override;
    virtual void visit(const AST::AssignExprAssignAdd & astNode) override;
    virtual void visit(const AST::AssignExprAssignARShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignBAnd & astNode) override;
    virtual void visit(const AST::AssignExprAssignBase & astNode) override;
    virtual void visit(const AST::AssignExprAssignBOr & astNode) override;
    virtual void visit(const AST::AssignExprAssignBXor & astNode) override;
    virtual void visit(const AST::AssignExprAssignDiv & astNode) override;
    virtual void visit(const AST::AssignExprAssignLRShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignLShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignMod & astNode) override;
    virtual void visit(const AST::AssignExprAssignMul & astNode) override;
    virtual void visit(const AST::AssignExprAssignSub & astNode) override;
    virtual void visit(const AST::AssignExprBinaryOpBase & astNode) override;
    virtual void visit(const AST::AssignExprNoAssign & astNode) override;
    virtual void visit(const AST::BoolLit & astNode) override;
    virtual void visit(const AST::BreakStmntNoCond & astNode) override;
    virtual void visit(const AST::BreakStmntWithCond & astNode) override;
    virtual void visit(const AST::CastExprCast & astNode) override;
    virtual void visit(const AST::CastExprNoCast & astNode) override;
    virtual void visit(const AST::CmpExprEQ & astNode) override;
    virtual void visit(const AST::CmpExprGE & astNode) override;
    virtual void visit(const AST::CmpExprGT & astNode) override;
    virtual void visit(const AST::CmpExprLE & astNode) override;
    virtual void visit(const AST::CmpExprLT & astNode) override;
    virtual void visit(const AST::CmpExprNE & astNode) override;
    virtual void visit(const AST::CmpExprNoOp & astNode) override;
    virtual void visit(const AST::CmpExprTwoOps & astNode) override;
    virtual void visit(const AST::DeclDefFunc & astNode) override;
    virtual void visit(const AST::DeclDefVarDecl & astNode) override;
    virtual void visit(const AST::Func & astNode) override;
    virtual void visit(const AST::FuncArg & astNode) override;
    virtual void visit(const AST::FuncCall & astNode) override;
    virtual void visit(const AST::FuncCallArgListMulti & astNode) override;
    virtual void visit(const AST::FuncCallArgListSingle & astNode) override;
    virtual void visit(const AST::Identifier & astNode) override;
    virtual void visit(const AST::IfStmntElse & astNode) override;
    virtual void visit(const AST::IfStmntElseIf & astNode) override;
    virtual void visit(const AST::IfStmntNoElse & astNode) override;
    virtual void visit(const AST::IntLit & astNode) override;
    virtual void visit(const AST::LAndExprAnd & astNode) override;
    virtual void visit(const AST::LAndExprNoOp & astNode) override;
    virtual void visit(const AST::LoopStmntNoCond & astNode) override;
    virtual void visit(const AST::LoopStmntWithCond & astNode) override;
    virtual void visit(const AST::LOrExprNoOp & astNode) override;
    virtual void visit(const AST::LOrExprOr & astNode) override;
    virtual void visit(const AST::Module & astNode) override;
    virtual void visit(const AST::MulExprBAnd & astNode) override;
    virtual void visit(const AST::MulExprDiv & astNode) override;
    virtual void visit(const AST::MulExprMod & astNode) override;
    virtual void visit(const AST::MulExprMul & astNode) override;
    virtual void visit(const AST::MulExprNoOp & astNode) override;
    virtual void visit(const AST::MulExprTwoOps & astNode) override;
    virtual void visit(const AST::NextStmntNoCond & astNode) override;
    virtual void visit(const AST::NextStmntWithCond & astNode) override;
    virtual void visit(const AST::NoOpStmnt & astNode) override;
    virtual void visit(const AST::NotExprBNot & astNode) override;
    virtual void visit(const AST::NotExprLNot & astNode) override;
    virtual void visit(const AST::NotExprNoOp & astNode) override;
    virtual void visit(const AST::PostfixExprArrayLookup & astNode) override;
    virtual void visit(const AST::PostfixExprDec & astNode) override;
    virtual void visit(const AST::PostfixExprFuncCall & astNode) override;
    virtual void visit(const AST::PostfixExprInc & astNode) override;
    virtual void visit(const AST::PostfixExprIncDecBase & astNode) override;
    virtual void visit(const AST::PostfixExprNoPostfix & astNode) override;
    virtual void visit(const AST::PrimaryExprArrayLit & astNode) override;
    virtual void visit(const AST::PrimaryExprBoolLit & astNode) override;
    virtual void visit(const AST::PrimaryExprIdentifier & astNode) override;
    virtual void visit(const AST::PrimaryExprIntLit & astNode) override;
    virtual void visit(const AST::PrimaryExprRandExpr & astNode) override;
    virtual void visit(const AST::PrimaryExprReadnum & astNode) override;
    virtual void visit(const AST::PrimaryExprStrLit & astNode) override;
    virtual void visit(const AST::PrimaryExprTime & astNode) override;
    virtual void visit(const AST::PrimitiveType & astNode) override;
    virtual void visit(const AST::PrintStmnt & astNode) override;
    virtual void visit(const AST::RandExprRand & astNode) override;
    virtual void visit(const AST::RandExprSRand & astNode) override;
    virtual void visit(const AST::ReadnumExpr & astNode) override;
    virtual void visit(const AST::ReturnStmntNoCondVoid & astNode) override;
    virtual void visit(const AST::ReturnStmntNoCondWithValue & astNode) override;
    virtual void visit(const AST::ReturnStmntWithCondAndValue & astNode) override;
    virtual void visit(const AST::ReturnStmntWithCondBase & astNode) override;
    virtual void visit(const AST::ReturnStmntWithCondVoid & astNode) override;
    virtual void visit(const AST::Scope & astNode) override;
    virtual void visit(const AST::ScopeStmnt & astNode) override;
    virtual void visit(const AST::ShiftExprARShift & astNode) override;
    virtual void visit(const AST::ShiftExprLRShift & astNode) override;
    virtual void visit(const AST::ShiftExprLShift & astNode) override;
    virtual void visit(const AST::ShiftExprNoOp & astNode) override;
    virtual void visit(const AST::ShiftExprTwoOps & astNode) override;
    virtual void visit(const AST::StmntAssertStmnt & astNode) override;
    virtual void visit(const AST::StmntAssignExpr & astNode) override;
    virtual void visit(const AST::StmntBreakStmnt & astNode) override;
    virtual void visit(const AST::StmntIfStmnt & astNode) override;
    virtual void visit(const AST::StmntLoopStmnt & astNode) override;
    virtual void visit(const AST::StmntNextStmnt & astNode) override;
    virtual void visit(const AST::StmntNoOpStmnt & astNode) override;
    virtual void visit(const AST::StmntPrintStmnt & astNode) override;
    virtual void visit(const AST::StmntReturnStmnt & astNode) override;
    virtual void visit(const AST::StmntScopeStmnt & astNode) override;
    virtual void visit(const AST::StmntVarDecl & astNode) override;
    virtual void visit(const AST::StmntWhileStmnt & astNode) override;
    virtual void visit(const AST::StrLit & astNode) override;
    virtual void visit(const AST::TernaryExprNoCond & astNode) override;
    virtual void visit(const AST::TernaryExprWithCond & astNode) override;
    virtual void visit(const AST::TimeExpr & astNode) override;
    virtual void visit(const AST::TypeArray & astNode) override;
    virtual void visit(const AST::TypePrimitive & astNode) override;
    virtual void visit(const AST::UnaryExprMinus & astNode) override;
    virtual void visit(const AST::UnaryExprParen & astNode) override;
    virtual void visit(const AST::UnaryExprPlus & astNode) override;
    virtual void visit(const AST::UnaryExprPlusMinusBase & astNode) override;
    virtual void visit(const AST::UnaryExprPrimary & astNode) override;
    virtual void visit(const AST::VarDeclExplicitType & astNode) override;
    virtual void visit(const AST::VarDeclInferType & astNode) override;
    virtual void visit(const AST::WhileStmnt & astNode) override;
    
private:
    CodegenCtx &        mCtx;
    ConstCodegen        mConstCodegen;
    CodegenDataType     mCodegenDataType;
    std::string         mModuleName;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
