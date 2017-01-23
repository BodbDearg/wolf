#include "Codegen.hpp"

#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCPostfixExpr.hpp"
#include "AST/Nodes/WCUnaryExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "DataTypeCodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenPlusUnaryOp : public DataTypeCodegenUnaryOp {
public:
    CodegenPlusUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
        DataTypeCodegenUnaryOp(cg, expr, "+", "plus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mExprVal);
    }
};

class CodegenMinusUnaryOp : public DataTypeCodegenUnaryOp {
public:
    CodegenMinusUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
        DataTypeCodegenUnaryOp(cg, expr, "-", "minus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateNeg(mExprVal, "Int64:Minus:Result"));
    }
};

void Codegen::visit(const AST::UnaryExprPrimary & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::UnaryExprPlus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenPlusUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::UnaryExprMinus & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMinusUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::UnaryExprParen & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
