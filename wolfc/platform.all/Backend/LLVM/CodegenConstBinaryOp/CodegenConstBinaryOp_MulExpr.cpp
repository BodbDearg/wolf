#include "CodegenConstBinaryOp_MulExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstMulBinaryOp
//-----------------------------------------------------------------------------
CodegenConstMulBinaryOp::CodegenConstMulBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "*", "multiply")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstMulBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getMul(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstDivBinaryOp
//-----------------------------------------------------------------------------
CodegenConstDivBinaryOp::CodegenConstDivBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "/", "divide")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstDivBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getSDiv(mLeftConst.mLLVMConst,
                                             mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstModBinaryOp
//-----------------------------------------------------------------------------
CodegenConstModBinaryOp::CodegenConstModBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "%", "modulus")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstModBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getSRem(mLeftConst.mLLVMConst,
                                             mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstBAndBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBAndBinaryOp::CodegenConstBAndBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "&", "bitwise and")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstBAndBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getAnd(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
