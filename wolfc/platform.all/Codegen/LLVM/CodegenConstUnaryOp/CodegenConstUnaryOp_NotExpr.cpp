#include "CodegenConstUnaryOp_NotExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenConstLNotUnaryOp::CodegenConstLNotUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenConstUnaryOp(cg, expr, "not", "logical not")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLNotUnaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getNot(mExprConst.mLLVMConst));
}

//-----------------------------------------------------------------------------
// CodegenConstBNotUnaryOp
//-----------------------------------------------------------------------------
CodegenConstBNotUnaryOp::CodegenConstBNotUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenConstUnaryOp(cg, expr, "~", "bitwise not")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstBNotUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getNot(mExprConst.mLLVMConst));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
