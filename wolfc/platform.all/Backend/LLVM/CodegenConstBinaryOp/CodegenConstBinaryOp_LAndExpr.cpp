//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstBinaryOp_LAndExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLAndBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLAndBinaryOp::CodegenConstLAndBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstLogicalBinaryOp(cg, leftExpr, rightExpr, "and", "logical and")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLAndBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    WC_ASSERT(mLeftConst.mLLVMConst);
    WC_ASSERT(mRightConst.mLLVMConst);
    pushOpResult(llvm::ConstantExpr::getAnd(mLeftConst.mLLVMConst, mRightConst.mLLVMConst));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
