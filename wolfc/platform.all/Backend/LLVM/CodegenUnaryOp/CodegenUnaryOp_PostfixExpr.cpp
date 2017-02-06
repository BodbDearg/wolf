#include "CodegenUnaryOp_PostfixExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenIncUnaryOp
//-----------------------------------------------------------------------------
CodegenIncUnaryOp::CodegenIncUnaryOp(Codegen & cg,
                                     const AST::ASTNode & expr,
                                     bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "++",
                   "increment",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenIncUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::ConstantInt * incByConstant = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCG.mCtx.mLLVMCtx), 1);
    WC_ASSERT(incByConstant);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateAdd(mExprVal.mLLVMVal, incByConstant));
}

//-----------------------------------------------------------------------------
// CodegenDecUnaryOp
//----------------------------------------------------------------------------
CodegenDecUnaryOp::CodegenDecUnaryOp(Codegen & cg,
                                     const AST::ASTNode & expr,
                                     bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "--",
                   "decrement",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenDecUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::ConstantInt * decByConstant = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCG.mCtx.mLLVMCtx), 1);
    WC_ASSERT(decByConstant);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateSub(mExprVal.mLLVMVal, decByConstant));
}
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
