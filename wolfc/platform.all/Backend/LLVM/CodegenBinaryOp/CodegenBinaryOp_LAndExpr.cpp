#include "CodegenBinaryOp_LAndExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLAndBinaryOp
//-----------------------------------------------------------------------------
CodegenLAndBinaryOp::CodegenLAndBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "and", "logical and")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenLAndBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateAnd(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Bool:LAnd:Result"));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
