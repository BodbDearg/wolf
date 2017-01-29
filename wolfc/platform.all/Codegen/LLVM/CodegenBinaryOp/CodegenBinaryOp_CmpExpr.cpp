#include "CodegenBinaryOp_CmpExpr.hpp"

#include "Codegen.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenCmpEQBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpEQBinaryOp::CodegenCmpEQBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "==", "equals")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenCmpEQBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpEQ(mLeftVal.mLLVMVal,
                                                  mRightVal.mLLVMVal,
                                                  "Bool:CmpEQ:Result"));
}

void CodegenCmpEQBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpEQ(mLeftVal.mLLVMVal,
                                                  mRightVal.mLLVMVal,
                                                  "Int64:CmpEQ:Result"));
}

//-----------------------------------------------------------------------------
// CodegenCmpNEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpNEBinaryOp::CodegenCmpNEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "!=", "not equals")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCmpNEBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpNE(mLeftVal.mLLVMVal,
                                                  mRightVal.mLLVMVal,
                                                  "Bool:CmpNE:Result"));
}

void CodegenCmpNEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpNE(mLeftVal.mLLVMVal,
                                                  mRightVal.mLLVMVal,
                                                  "Int64:CmpNE:Result"));
}

//-----------------------------------------------------------------------------
// CodegenCmpLTBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpLTBinaryOp::CodegenCmpLTBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "<", "less than")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCmpLTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSLT(mLeftVal.mLLVMVal,
                                                   mRightVal.mLLVMVal,
                                                   "Int64:CmpLT:Result"));
}

//-----------------------------------------------------------------------------
// CodegenCmpLEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpLEBinaryOp::CodegenCmpLEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "<=", "less than or equal to")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCmpLEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSLE(mLeftVal.mLLVMVal,
                                                   mRightVal.mLLVMVal,
                                                   "Int64:CmpLE:Result"));
}

//-----------------------------------------------------------------------------
// CodegenCmpGTBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpGTBinaryOp::CodegenCmpGTBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, ">", "greater than")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCmpGTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSGT(mLeftVal.mLLVMVal,
                                                   mRightVal.mLLVMVal,
                                                   "Int64:CmpGT:Result"));
}

//-----------------------------------------------------------------------------
// CodegenCmpGEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpGEBinaryOp::CodegenCmpGEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, ">=", "greater than or equal to")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCmpGEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateICmpSGE(mLeftVal.mLLVMVal,
                                                   mRightVal.mLLVMVal,
                                                   "Int64:CmpGE:Result"));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
