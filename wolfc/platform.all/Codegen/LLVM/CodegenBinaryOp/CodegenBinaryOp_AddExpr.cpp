#include "CodegenBinaryOp_AddExpr.hpp"

#include "Codegen.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenAddBinaryOp
//-----------------------------------------------------------------------------
CodegenAddBinaryOp::CodegenAddBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "+", "add")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenAddBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateAdd(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:Add:Result"));
}

//-----------------------------------------------------------------------------
// CodegenSubBinaryOp
//-----------------------------------------------------------------------------
CodegenSubBinaryOp::CodegenSubBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "-", "subtract")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenSubBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateSub(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:Sub:Result"));
}

//-----------------------------------------------------------------------------
// CodegenBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBOrBinaryOp::CodegenBOrBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "|", "bitwise or")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenBOrBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateOr(mLeftVal.mLLVMVal,
                                              mRightVal.mLLVMVal,
                                              "Int64:BOr:Result"));
}

//-----------------------------------------------------------------------------
// CodegenBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBXOrBinaryOp::CodegenBXOrBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "^", "bitwise xor")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenBXOrBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateXor(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:BXOr:Result"));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
