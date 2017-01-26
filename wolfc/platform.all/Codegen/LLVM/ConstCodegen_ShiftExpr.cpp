#include "ConstCodegen.hpp"

#include "AST/Nodes/WCShiftExpr.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "CodegenConstBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstLShiftBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstLShiftBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, "<<", "left shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getShl(mLeftVal, mRightVal));
    }
};

class CodegenConstARShiftBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstARShiftBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, ">>", "arithmetic right shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getAShr(mLeftVal, mRightVal));
    }
};

class CodegenConstLRShiftBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstLRShiftBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, ">>>", "logical right shift")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getLShr(mLeftVal, mRightVal));
    }
};

void ConstCodegen::visit(const AST::ShiftExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::ShiftExprLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::ShiftExprARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::ShiftExprLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
