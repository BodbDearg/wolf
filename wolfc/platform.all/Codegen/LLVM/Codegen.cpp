#include "Codegen.hpp"

#include "AST/Nodes/WCArrayLit.hpp"
#include "AST/Nodes/WCArrayLitExprs.hpp"
#include "AST/Nodes/WCAssertStmnt.hpp"
#include "AST/Nodes/WCBreakStmnt.hpp"
#include "AST/Nodes/WCFuncArg.hpp"
#include "AST/Nodes/WCFuncCall.hpp"
#include "AST/Nodes/WCFuncCallArgList.hpp"
#include "AST/Nodes/WCIBasicCodegenNode.hpp"
#include "AST/Nodes/WCIdentifier.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "AST/Nodes/WCIfStmnt.hpp"
#include "AST/Nodes/WCLoopStmnt.hpp"
#include "AST/Nodes/WCNextStmnt.hpp"
#include "AST/Nodes/WCNoOpStmnt.hpp"
#include "AST/Nodes/WCRandExpr.hpp"
#include "AST/Nodes/WCReadnumExpr.hpp"
#include "AST/Nodes/WCReturnStmnt.hpp"
#include "AST/Nodes/WCScopeStmnt.hpp"
#include "AST/Nodes/WCStrLit.hpp"
#include "AST/Nodes/WCTimeExpr.hpp"
#include "AST/Nodes/WCVarDecl.hpp"
#include "AST/Nodes/WCWhileStmnt.hpp"
#include "CodegenCtx.hpp"

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

void Codegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprs & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ArrayLitExprsSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::AssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListMulti & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::FuncCallArgListSingle & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntElseIf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::IfStmntNoElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::LoopStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NextStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::NoOpStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::RandExprSRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReadnumExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntNoCondWithValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::StrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::TimeExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclExplicitType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::VarDeclInferType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::WhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::codegenBinaryOp(AST::ASTNode & leftExpr,
                              AST::ASTNode & rightExpr,
                              CodegenBinaryOpFunc codegenFunc,
                              const char * opName,
                              const char * opSymbol)
{
    WC_ASSERT(codegenFunc);
    WC_ASSERT(opName);
    WC_ASSERT(opSymbol);
    
    // Both left and right must be expressions:
    AST::IExpr * leftAsExpr = dynamic_cast<AST::IExpr*>(&leftExpr);
    
    if (!leftAsExpr) {
        mCtx.error(leftExpr,
                   "Left side of '%s' (%s) operator must be an expression with a value result!",
                   opSymbol,
                   opName);
    }
    
    AST::IExpr * rightAsExpr = dynamic_cast<AST::IExpr*>(&rightExpr);
    
    if (!rightAsExpr) {
        mCtx.error(leftExpr,
                   "Right side of '%s' (%s) operator must be an expression with a value result!",
                   opSymbol,
                   opName);
    }
    
    WC_GUARD(leftAsExpr && rightAsExpr);
    
    // Okay, codegen both the left and right expressions
    leftExpr.accept(*this);
    llvm::Value * leftValue = mCtx.popLLVMValue();
    rightExpr.accept(*this);
    llvm::Value * rightValue = mCtx.popLLVMValue();
    
    // If both are okay then call the codegen function
    if (leftValue && rightValue) {
        DataType & exprDataType = leftAsExpr->dataType();
        codegenFunc(*this,
                    exprDataType,
                    leftExpr,
                    *leftValue,
                    rightExpr,
                    *rightValue,
                    opName,
                    opSymbol);
    }
}

void Codegen::issueBinaryOpNotSupportedError(AST::ASTNode & leftExpr,
                                             AST::ASTNode & rightExpr,
                                             const char * opName,
                                             const char * opSymbol)
{
    // Both left and right must be expressions:
    AST::IExpr * leftAsExpr = dynamic_cast<AST::IExpr*>(&leftExpr);
    
    if (!leftAsExpr) {
        mCtx.error(leftExpr,
                   "Left side of '%s' (%s) operator must be an expression with a value result!",
                   opSymbol,
                   opName);
    }
    
    AST::IExpr * rightAsExpr = dynamic_cast<AST::IExpr*>(&rightExpr);
    
    if (!rightAsExpr) {
        mCtx.error(leftExpr,
                   "Right side of '%s' (%s) operator must be an expression with a value result!",
                   opSymbol,
                   opName);
    }
    
    WC_GUARD(leftAsExpr && rightAsExpr);
    
    // Issue the error. Issue it at the parent of the binary pair:
    AST::ASTNode * parent = leftExpr.mParent;
    WC_ASSERT(parent);
    mCtx.error(*parent,
               "Binary operator '%s' (%s) is not supported for a left expression "
               "of type '%s' and right expression of type '%s'!",
               leftAsExpr->dataType().name().c_str(),
               rightAsExpr->dataType().name().c_str());
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
