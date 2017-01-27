#include "Codegen.hpp"

#include "AST/Nodes/WCLAndExpr.hpp"
#include "AST/Nodes/WCLOrExpr.hpp"
#include "CodegenBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLOrBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenLOrBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "or", "logical or")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateOr(mLeftVal.mLLVMVal,
                                                  mRightVal.mLLVMVal,
                                                  "Bool:LOr:Result"));
    }
};

void Codegen::visit(const AST::LOrExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::LOrExprOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
