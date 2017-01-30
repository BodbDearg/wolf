#include "CodegenConstUnaryOp_UnaryExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstPlusUnaryOp
//-----------------------------------------------------------------------------
CodegenConstPlusUnaryOp::CodegenConstPlusUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenConstUnaryOp(cg, expr, "+", "plus")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstPlusUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mExprConst.mLLVMConst);
}

//-----------------------------------------------------------------------------
// CodegenConstMinusUnaryOp
//-----------------------------------------------------------------------------
CodegenConstMinusUnaryOp::CodegenConstMinusUnaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & expr)
:
    CodegenConstUnaryOp(cg, expr, "-", "minus")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstMinusUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getNeg(mExprConst.mLLVMConst));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
