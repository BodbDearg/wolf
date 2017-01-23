#include "Codegen.hpp"

#include "AST/Nodes/WCLAndExpr.hpp"
#include "AST/Nodes/WCNotExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "DataTypeCodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLAndBinaryOp : public DataTypeCodegenBinaryOp {
public:
    CodegenLAndBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        DataTypeCodegenBinaryOp(cg, leftExpr, rightExpr, "and", "logical and")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateAnd(mLeftVal, mRightVal, "Bool:LAnd:Result"));
    }
};

void Codegen::visit(const AST::LAndExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::LAndExprAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
