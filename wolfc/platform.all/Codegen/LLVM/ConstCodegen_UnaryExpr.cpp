#include "ConstCodegen.hpp"

#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCPostfixExpr.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "CodegenConstUnaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstPlusUnaryOp : public CodegenConstUnaryOpHelper {
public:
    CodegenConstPlusUnaryOp(ConstCodegen & cg, const AST::ASTNode & expr) :
        CodegenConstUnaryOpHelper(cg, expr, "+", "plus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mExprConst.mLLVMConst);
    }
};

class CodegenConstMinusUnaryOp : public CodegenConstUnaryOpHelper {
public:
    CodegenConstMinusUnaryOp(ConstCodegen & cg, const AST::ASTNode & expr) :
        CodegenConstUnaryOpHelper(cg, expr, "-", "minus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getNeg(mExprConst.mLLVMConst));
    }
};

void ConstCodegen::visit(const AST::UnaryExprPrimary & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstPlusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstMinusUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
