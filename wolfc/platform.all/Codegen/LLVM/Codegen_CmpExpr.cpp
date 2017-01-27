#include "Codegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCCmpExpr.hpp"
#include "CodegenBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenCmpEQBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpEQBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "==", "equals")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpEQ(mLeftVal.mLLVMVal,
                                                      mRightVal.mLLVMVal,
                                                      "Bool:CmpEQ:Result"));
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpEQ(mLeftVal.mLLVMVal,
                                                      mRightVal.mLLVMVal,
                                                      "Int64:CmpEQ:Result"));
    }
};

class CodegenCmpNEBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpNEBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "!=", "not equals")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpNE(mLeftVal.mLLVMVal,
                                                      mRightVal.mLLVMVal,
                                                      "Bool:CmpNE:Result"));
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpNE(mLeftVal.mLLVMVal,
                                                      mRightVal.mLLVMVal,
                                                      "Int64:CmpNE:Result"));
    }
};

class CodegenCmpLTBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpLTBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "<", "less than")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSLT(mLeftVal.mLLVMVal,
                                                       mRightVal.mLLVMVal,
                                                       "Int64:CmpLT:Result"));
    }
};

class CodegenCmpLEBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpLEBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "<=", "less than or equal to")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSLE(mLeftVal.mLLVMVal,
                                                       mRightVal.mLLVMVal,
                                                       "Int64:CmpLE:Result"));
    }
};

class CodegenCmpGTBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpGTBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, ">", "greater than")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSGT(mLeftVal.mLLVMVal,
                                                       mRightVal.mLLVMVal,
                                                       "Int64:CmpGT:Result"));
    }
};

class CodegenCmpGEBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenCmpGEBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, ">=", "greater than or equal to")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSGE(mLeftVal.mLLVMVal,
                                                       mRightVal.mLLVMVal,
                                                       "Int64:CmpGE:Result"));
    }
};

void Codegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpEQBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpNEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpLTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpLEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpGTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenCmpGEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
