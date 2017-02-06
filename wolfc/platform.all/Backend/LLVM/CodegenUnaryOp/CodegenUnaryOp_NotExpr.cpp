#include "CodegenUnaryOp_NotExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenLNotUnaryOp::CodegenLNotUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "not",
                   "logical not",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenLNotUnaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateNot(mExprVal.mLLVMVal, "Bool:LNot:Result"));
}

//-----------------------------------------------------------------------------
// CodegenBNotUnaryOp
//-----------------------------------------------------------------------------
CodegenBNotUnaryOp::CodegenBNotUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "~",
                   "bitwise not",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenBNotUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateNot(mExprVal.mLLVMVal, "Int64:BNot:Result"));
}
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
