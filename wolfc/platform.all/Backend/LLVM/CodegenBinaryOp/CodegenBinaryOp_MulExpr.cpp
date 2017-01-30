#include "CodegenBinaryOp_MulExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenMulBinaryOp
//-----------------------------------------------------------------------------
CodegenMulBinaryOp::CodegenMulBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "*", "multiply")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenMulBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateMul(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:Mul:Result"));
}

//-----------------------------------------------------------------------------
// CodegenDivBinaryOp
//-----------------------------------------------------------------------------
CodegenDivBinaryOp::CodegenDivBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "/", "divide")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenDivBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateSDiv(mLeftVal.mLLVMVal,
                                                mRightVal.mLLVMVal,
                                                "Int64:Div:Result"));
}

//-----------------------------------------------------------------------------
// CodegenModBinaryOp
//-----------------------------------------------------------------------------
CodegenModBinaryOp::CodegenModBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "%", "modulus")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenModBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateSRem(mLeftVal.mLLVMVal,
                                                mRightVal.mLLVMVal,
                                                "Int64:Mod:Result"));
}

//-----------------------------------------------------------------------------
// CodegenBAndBinaryOp
//-----------------------------------------------------------------------------
CodegenBAndBinaryOp::CodegenBAndBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "&", "bitwise and")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenBAndBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateAnd(mLeftVal.mLLVMVal,
                                               mRightVal.mLLVMVal,
                                               "Int64:BAnd:Result"));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
