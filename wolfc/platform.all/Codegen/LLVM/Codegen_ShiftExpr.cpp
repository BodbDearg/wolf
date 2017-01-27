#include "Codegen.hpp"

#include "AST/Nodes/WCShiftExpr.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "CodegenBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLShiftBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenLShiftBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "<<", "left shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateShl(mLeftVal.mLLVMVal,
                                                   mRightVal.mLLVMVal,
                                                   "Int64:LShift:Result"));
    }
};

class CodegenARShiftBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenARShiftBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, ">>", "arithmetic right shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateAShr(mLeftVal.mLLVMVal,
                                                    mRightVal.mLLVMVal,
                                                    "Int64:ARShift:Result"));
    }
};

class CodegenLRShiftBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenLRShiftBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, ">>>", "logical right shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateLShr(mLeftVal.mLLVMVal,
                                                    mRightVal.mLLVMVal,
                                                    "Int64:LRShift:Result"));
    }
};

void Codegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
