#include "CodegenConstBinaryOp_LOrExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLAndBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLOrBinaryOp::CodegenConstLOrBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr, 
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "or", "logical or")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLOrBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getOr(mLeftConst.mLLVMConst,
                                           mRightConst.mLLVMConst));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
