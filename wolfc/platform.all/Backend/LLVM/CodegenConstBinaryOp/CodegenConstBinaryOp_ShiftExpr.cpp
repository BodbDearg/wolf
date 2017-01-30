#include "CodegenConstBinaryOp_ShiftExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLShiftBinaryOp::CodegenConstLShiftBinaryOp(ConstCodegen & cg,
                                                       const AST::ASTNode & leftExpr,
                                                       const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "<<", "left shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getShl(mLeftConst.mLLVMConst,
                                            mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstARShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstARShiftBinaryOp::CodegenConstARShiftBinaryOp(ConstCodegen & cg,
                                                         const AST::ASTNode & leftExpr,
                                                         const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, ">>", "arithmetic right shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstARShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getAShr(mLeftConst.mLLVMConst,
                                             mRightConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstLRShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLRShiftBinaryOp::CodegenConstLRShiftBinaryOp(ConstCodegen & cg,
                                                         const AST::ASTNode & leftExpr,
                                                         const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, ">>>", "logical right shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLRShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getLShr(mLeftConst.mLLVMConst,
                                             mRightConst.mLLVMConst));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
