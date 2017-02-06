#include "CodegenUnaryOp_UnaryExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenPlusUnaryOp::CodegenPlusUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "+",
                   "plus",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenPlusUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mExprVal.mLLVMVal);
}

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//----------------------------------------------------------------------------
CodegenMinusUnaryOp::CodegenMinusUnaryOp(Codegen & cg,
                                         const AST::ASTNode & expr,
                                         bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "-",
                   "minus",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenMinusUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateNeg(mExprVal.mLLVMVal, "Int64:Minus:Result"));
}
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
