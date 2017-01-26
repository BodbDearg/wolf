#include "ConstCodegen.hpp"

#include "AST/Nodes/WCMulExpr.hpp"
#include "AST/Nodes/WCShiftExpr.hpp"
#include "CodegenConstBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstMulBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstMulBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "*", "multiply")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getMul(mLeftVal, mRightVal));
    }
};

class CodegenConstDivBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstDivBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "/", "divide")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getSDiv(mLeftVal, mRightVal));
    }
};

class CodegenConstModBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstModBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "%", "modulus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getSRem(mLeftVal, mRightVal));
    }
};

class CodegenConstBAndBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstBAndBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "&", "bitwise and")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getAnd(mLeftVal, mRightVal));
    }
};

void ConstCodegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstModBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
