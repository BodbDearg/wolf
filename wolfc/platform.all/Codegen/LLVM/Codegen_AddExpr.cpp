#include "Codegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

static void codegenAddOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateAdd(&leftValue, &rightValue, "Int:Add:Result");
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

static void codegenSubOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateSub(&leftValue, &rightValue, "Int:Sub:Result");
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

static void codegenBOrOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateOr(&leftValue, &rightValue, "Int:BOr:Result");
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

static void codegenBXOrOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateXor(&leftValue, &rightValue, "Int:BXor:Result");
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

void Codegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenAddOp, "add", "+");
}

void Codegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenSubOp, "subtract", "-");
}

void Codegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenBOrOp, "bitwise or", "|");
}

void Codegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenBXOrOp, "bitwise xor", "^");
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
