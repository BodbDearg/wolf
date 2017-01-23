#include "Codegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "CodegenBinaryOp.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenAddBinaryOp : public CodegenBinaryOp {
public:
    CodegenAddBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOp(cg, leftExpr, rightExpr, "+", "add")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateAdd(mLeftVal, mRightVal, "Int64:Add:Result"));
    }
};

class CodegenSubBinaryOp : public CodegenBinaryOp {
public:
    CodegenSubBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOp(cg, leftExpr, rightExpr, "-", "subtract")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateSub(mLeftVal, mRightVal, "Int64:Sub:Result"));
    }
};

class CodegenBOrBinaryOp : public CodegenBinaryOp {
public:
    CodegenBOrBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOp(cg, leftExpr, rightExpr, "|", "bitwise or")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateOr(mLeftVal, mRightVal, "Int64:BOr:Result"));
    }
};

class CodegenBXOrBinaryOp : public CodegenBinaryOp {
public:
    CodegenBXOrBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOp(cg, leftExpr, rightExpr, "^", "bitwise xor")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateXor(mLeftVal, mRightVal, "Int64:BXOr:Result"));
    }
};

void Codegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenAddBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenSubBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBXOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
