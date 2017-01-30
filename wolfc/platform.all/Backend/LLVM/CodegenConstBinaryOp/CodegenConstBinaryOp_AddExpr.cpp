#include "CodegenConstBinaryOp_AddExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstAddBinaryOp
//-----------------------------------------------------------------------------
CodegenConstAddBinaryOp::CodegenConstAddBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "+", "add")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstAddBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getAdd(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstSubBinaryOp
//-----------------------------------------------------------------------------
CodegenConstSubBinaryOp::CodegenConstSubBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "-", "subtract")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstSubBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getSub(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBOrBinaryOp::CodegenConstBOrBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "|", "bitwise or")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstBOrBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getOr(mLeftConst.mLLVMConst,
                                           mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBXOrBinaryOp::CodegenConstBXOrBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "^", "bitwise xor")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstBXOrBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getXor(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
