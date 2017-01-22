#include "Codegen.hpp"

#include "AST/Nodes/WCMulExpr.hpp"
#include "AST/Nodes/WCShiftExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

static void codegenMulOp(Codegen & cg,
                         DataType & exprDataType,
                         AST::ASTNode & leftExprASTNode,
                         llvm::Value & leftValue,
                         AST::ASTNode & rightExprASTNode,
                         llvm::Value & rightValue,
                         const char * opName,
                         const char * opSymbol)
{
    switch (exprDataType.getTypeId()) {
        case DataTypeId::kInt64: {
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateMul(&leftValue, &rightValue, "Int:Mul:Result");
            WC_ASSERT(value);
            cg.mCtx.pushLLVMValue(*value);
        }   break;
            
        case DataTypeId::kUnknown:
        case DataTypeId::kVoid:
        case DataTypeId::kBool:
        case DataTypeId::kStr:
        case DataTypeId::kArray:
        case DataTypeId::kUnknownArray:
            cg.issueBinaryOpNotSupportedError(leftExprASTNode, rightExprASTNode, opName, opSymbol);
            break;
    }
}

static void codegenDivOp(Codegen & cg,
                         DataType & exprDataType,
                         AST::ASTNode & leftExprASTNode,
                         llvm::Value & leftValue,
                         AST::ASTNode & rightExprASTNode,
                         llvm::Value & rightValue,
                         const char * opName,
                         const char * opSymbol)
{
    switch (exprDataType.getTypeId()) {
        case DataTypeId::kInt64: {
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateSDiv(&leftValue, &rightValue, "Int:Div:Result");
            WC_ASSERT(value);
            cg.mCtx.pushLLVMValue(*value);
        }   break;
            
        case DataTypeId::kUnknown:
        case DataTypeId::kVoid:
        case DataTypeId::kBool:
        case DataTypeId::kStr:
        case DataTypeId::kArray:
        case DataTypeId::kUnknownArray:
            cg.issueBinaryOpNotSupportedError(leftExprASTNode, rightExprASTNode, opName, opSymbol);
            break;
    }
}

static void codegenModOp(Codegen & cg,
                         DataType & exprDataType,
                         AST::ASTNode & leftExprASTNode,
                         llvm::Value & leftValue,
                         AST::ASTNode & rightExprASTNode,
                         llvm::Value & rightValue,
                         const char * opName,
                         const char * opSymbol)
{
    switch (exprDataType.getTypeId()) {
        case DataTypeId::kInt64: {
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateSRem(&leftValue, &rightValue, "Int:Mod:Result");
            WC_ASSERT(value);
            cg.mCtx.pushLLVMValue(*value);
        }   break;
            
        case DataTypeId::kUnknown:
        case DataTypeId::kVoid:
        case DataTypeId::kBool:
        case DataTypeId::kStr:
        case DataTypeId::kArray:
        case DataTypeId::kUnknownArray:
            cg.issueBinaryOpNotSupportedError(leftExprASTNode, rightExprASTNode, opName, opSymbol);
            break;
    }
}

static void codegenBAndOp(Codegen & cg,
                          DataType & exprDataType,
                          AST::ASTNode & leftExprASTNode,
                          llvm::Value & leftValue,
                          AST::ASTNode & rightExprASTNode,
                          llvm::Value & rightValue,
                          const char * opName,
                          const char * opSymbol)
{
    switch (exprDataType.getTypeId()) {
        case DataTypeId::kInt64: {
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateAnd(&leftValue, &rightValue, "Int:BAnd:Result");
            WC_ASSERT(value);
            cg.mCtx.pushLLVMValue(*value);
        }   break;
            
        case DataTypeId::kUnknown:
        case DataTypeId::kVoid:
        case DataTypeId::kBool:
        case DataTypeId::kStr:
        case DataTypeId::kArray:
        case DataTypeId::kUnknownArray:
            cg.issueBinaryOpNotSupportedError(leftExprASTNode, rightExprASTNode, opName, opSymbol);
            break;
    }
}

void Codegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenMulOp, "multiply", "*");
}

void Codegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenDivOp, "divide", "/");
}

void Codegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenModOp, "modulus", "%");
}

void Codegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenBAndOp, "bitwise and", "&");
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
