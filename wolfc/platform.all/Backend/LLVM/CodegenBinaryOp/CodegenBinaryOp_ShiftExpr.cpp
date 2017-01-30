#include "CodegenBinaryOp_ShiftExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenLShiftBinaryOp::CodegenLShiftBinaryOp(Codegen & cg,
                                             const AST::ASTNode & leftExpr,
                                             const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "<<", "left shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenLShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateShl(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:LShift:Result"));
}

//-----------------------------------------------------------------------------
// CodegenARShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenARShiftBinaryOp::CodegenARShiftBinaryOp(Codegen & cg,
                                               const AST::ASTNode & leftExpr,
                                               const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, ">>", "arithmetic right shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenARShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateAShr(mLeftVal.mLLVMVal,
                                                mRightVal.mLLVMVal,
                                                "Int64:ARShift:Result"));
}

//-----------------------------------------------------------------------------
// CodegenLRShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenLRShiftBinaryOp::CodegenLRShiftBinaryOp(Codegen & cg,
                                               const AST::ASTNode & leftExpr,
                                               const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, ">>>", "logical right shift")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenLRShiftBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateLShr(mLeftVal.mLLVMVal,
                                                mRightVal.mLLVMVal,
                                                "Int64:LRShift:Result"));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
