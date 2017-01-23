#include "Codegen.hpp"

#include "AST/Nodes/WCCmpExpr.hpp"
#include "AST/Nodes/WCNotExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "DataTypeCodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLNotUnaryOp : public DataTypeCodegenUnaryOp {
public:
    CodegenLNotUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
        DataTypeCodegenUnaryOp(cg, expr, "not", "logical not")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateNot(mExprVal, "Bool:LNot:Result"));
    }
};

class CodegenBNotUnaryOp : public DataTypeCodegenUnaryOp {
public:
    CodegenBNotUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
        DataTypeCodegenUnaryOp(cg, expr, "~", "bitwise not")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateNot(mExprVal, "Int64:BNot:Result"));
    }
};

void Codegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLNotUnaryOp(*this, astNode.mExpr).codegen();
}

void Codegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBNotUnaryOp(*this, astNode.mExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
