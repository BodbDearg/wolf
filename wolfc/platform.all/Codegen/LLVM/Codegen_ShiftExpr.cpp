#include "Codegen.hpp"

#include "AST/Nodes/WCShiftExpr.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

static void codegenLShiftOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateShl(&leftValue, &rightValue, "Int:LShift:Result");
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

static void codegenARShiftOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateAShr(&leftValue, &rightValue, "Int:ARShift:Result");
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

static void codegenLRShiftOp(Codegen & cg,
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
            llvm::Value * value = cg.mCtx.mIRBuilder.CreateLShr(&leftValue, &rightValue, "Int::LRShift:Result");
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

void Codegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenLShiftOp, "left shift", "<<");
}

void Codegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenARShiftOp, "arithmetic right shift", ">>");
}

void Codegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenBinaryOp(astNode.mLeftExpr, astNode.mRightExpr, codegenLRShiftOp, "logical right shift", ">>>");
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
