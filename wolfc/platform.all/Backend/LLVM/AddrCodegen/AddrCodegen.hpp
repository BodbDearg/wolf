//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "AST/ASTNodeVisitor.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;
class CodegenCtx;
class CodegenDataType;
class ConstCodegen;

/**
 * Code generation which generates the address of supported nodes. Address code generation is 
 * required to lookup variables for load/store and also for array indexing etc.
 */
class AddrCodegen final : public AST::ASTNodeVisitor {
public:
    AddrCodegen(CodegenCtx & ctx,
                Codegen & codegen,
                ConstCodegen & constCodegen,
                CodegenDataType & codegenDataType);
    
    /* ASTNode visitor functions */
    virtual void visit(const AST::AddExprAdd & astNode) override;
    virtual void visit(const AST::AddExprBOr & astNode) override;
    virtual void visit(const AST::AddExprBXor & astNode) override;
    virtual void visit(const AST::AddExprNoOp & astNode) override;
    virtual void visit(const AST::AddExprSub & astNode) override;
    virtual void visit(const AST::ArrayLit & astNode) override;
    virtual void visit(const AST::AssertStmnt & astNode) override;
    virtual void visit(const AST::AssignExprAssign & astNode) override;
    virtual void visit(const AST::AssignExprAssignARShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignAdd & astNode) override;
    virtual void visit(const AST::AssignExprAssignBAnd & astNode) override;
    virtual void visit(const AST::AssignExprAssignBOr & astNode) override;
    virtual void visit(const AST::AssignExprAssignBXor & astNode) override;
    virtual void visit(const AST::AssignExprAssignDiv & astNode) override;
    virtual void visit(const AST::AssignExprAssignLRShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignLShift & astNode) override;
    virtual void visit(const AST::AssignExprAssignMul & astNode) override;
    virtual void visit(const AST::AssignExprAssignRem & astNode) override;
    virtual void visit(const AST::AssignExprAssignSub & astNode) override;
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
    virtual void visit(const AST::DeclDefFunc & astNode) override;
    virtual void visit(const AST::DeclDefVarDecl & astNode) override;
    virtual void visit(const AST::Func & astNode) override;
    virtual void visit(const AST::FuncArg & astNode) override;
    virtual void visit(const AST::Identifier & astNode) override;
    virtual void visit(const AST::IfStmntElse & astNode) override;
    virtual void visit(const AST::IfStmntElseIf & astNode) override;
    virtual void visit(const AST::IfStmntNoElse & astNode) override;
    virtual void visit(const AST::IntLit & astNode) override;
    virtual void visit(const AST::LAndExprAnd & astNode) override;
    virtual void visit(const AST::LAndExprNoOp & astNode) override;
    virtual void visit(const AST::LOrExprNoOp & astNode) override;
    virtual void visit(const AST::LOrExprOr & astNode) override;
    virtual void visit(const AST::LoopStmntNoCond & astNode) override;
    virtual void visit(const AST::LoopStmntWithCond & astNode) override;
    virtual void visit(const AST::Module & astNode) override;
    virtual void visit(const AST::MulExprBAnd & astNode) override;
    virtual void visit(const AST::MulExprDiv & astNode) override;
    virtual void visit(const AST::MulExprMul & astNode) override;
    virtual void visit(const AST::MulExprNoOp & astNode) override;
    virtual void visit(const AST::MulExprRem & astNode) override;
    virtual void visit(const AST::NextStmntNoCond & astNode) override;
    virtual void visit(const AST::NextStmntWithCond & astNode) override;
    virtual void visit(const AST::NoOpStmnt & astNode) override;
    virtual void visit(const AST::NotExprBNot & astNode) override;
    virtual void visit(const AST::NotExprLNot & astNode) override;
    virtual void visit(const AST::NotExprNoOp & astNode) override;
    virtual void visit(const AST::NullLit & astNode) override;
    virtual void visit(const AST::PostfixExprArrayLookup & astNode) override;
    virtual void visit(const AST::PostfixExprDec & astNode) override;
    virtual void visit(const AST::PostfixExprFuncCall & astNode) override;
    virtual void visit(const AST::PostfixExprInc & astNode) override;
    virtual void visit(const AST::PostfixExprNoOp & astNode) override;
    virtual void visit(const AST::PrefixExprAddrOf & astNode) override;
    virtual void visit(const AST::PrefixExprMinus & astNode) override;
    virtual void visit(const AST::PrefixExprNoOp & astNode) override;
    virtual void visit(const AST::PrefixExprPlus & astNode) override;
    virtual void visit(const AST::PrefixExprPtrDeref & astNode) override;
    virtual void visit(const AST::PrimaryExprArrayLit & astNode) override;
    virtual void visit(const AST::PrimaryExprBoolLit & astNode) override;
    virtual void visit(const AST::PrimaryExprIdentifier & astNode) override;
    virtual void visit(const AST::PrimaryExprIntLit & astNode) override;
    virtual void visit(const AST::PrimaryExprNullLit & astNode) override;
    virtual void visit(const AST::PrimaryExprParen & astNode) override;
    virtual void visit(const AST::PrimaryExprRandExpr & astNode) override;
    virtual void visit(const AST::PrimaryExprReadnumExpr & astNode) override;
    virtual void visit(const AST::PrimaryExprStrLit & astNode) override;
    virtual void visit(const AST::PrimaryExprTimeExpr & astNode) override;
    virtual void visit(const AST::PrimitiveType & astNode) override;
    virtual void visit(const AST::PrintStmnt & astNode) override;
    virtual void visit(const AST::RandExprRand & astNode) override;
    virtual void visit(const AST::RandExprSRand & astNode) override;
    virtual void visit(const AST::ReadnumExpr & astNode) override;
    virtual void visit(const AST::ReturnStmntNoCondVoid & astNode) override;
    virtual void visit(const AST::ReturnStmntNoCondWithValue & astNode) override;
    virtual void visit(const AST::ReturnStmntWithCondAndValue & astNode) override;
    virtual void visit(const AST::ReturnStmntWithCondVoid & astNode) override;
    virtual void visit(const AST::Scope & astNode) override;
    virtual void visit(const AST::ScopeStmnt & astNode) override;
    virtual void visit(const AST::ShiftExprARShift & astNode) override;
    virtual void visit(const AST::ShiftExprLRShift & astNode) override;
    virtual void visit(const AST::ShiftExprLShift & astNode) override;
    virtual void visit(const AST::ShiftExprNoOp & astNode) override;
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
    virtual void visit(const AST::TypePtr & astNode) override;
    virtual void visit(const AST::VarDeclExplicitType & astNode) override;
    virtual void visit(const AST::VarDeclInferType & astNode) override;
    virtual void visit(const AST::WhileStmnt & astNode) override;
    
    /* The codegen context */
    CodegenCtx & mCtx;
    
    /* The regular code generator */
    Codegen & mCodegen;
    
    /* Code generator in charge of constant code generation */
    ConstCodegen & mConstCodegen;
    
    /* Code generator in charge of data type generation */
    CodegenDataType & mCodegenDataType;
     
private:
    /* Issue a codegen not supported error for the given ASTNode type */
    void codegenNotSupportedForNodeTypeError(const AST::ASTNode & node,
                                             const char * nodeClassName);

    /* Issue a codegen error that a unary operation can't have it's address taken. */
    void cantTakeAddressOfUnaryOpError(const AST::ASTNode & exprNode,
                                       const char * opSymbol,
                                       const char * opName);
    
    /* Issue a codegen error that a binary operation can't have it's address taken. */
    void cantTakeAddressOfBinaryOpError(const AST::ASTNode & leftExprNode,
                                        const AST::ASTNode & rightExprNode,
                                        const char * opSymbol,
                                        const char * opName);
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
