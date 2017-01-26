#include "ConstCodegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCMulExpr.hpp"
#include "CodegenConstBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstAddBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstAddBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "+", "add")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getAdd(mLeftVal, mRightVal));
    }
};

class CodegenConstSubBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstSubBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "-", "subtract")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getSub(mLeftVal, mRightVal));
    }
};

class CodegenConstBOrBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstBOrBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "|", "bitwise or")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getOr(mLeftVal, mRightVal));
    }
};

class CodegenConstBXOrBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstBXOrBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "^", "bitwise xor")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getXor(mLeftVal, mRightVal));
    }
};

void ConstCodegen::visit(const AST::AddExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::AddExprAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstAddBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstSubBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::AddExprBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBXOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
