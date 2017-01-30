#include "CodegenBinaryOp_LOrExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLOrBinaryOp
//-----------------------------------------------------------------------------
CodegenLOrBinaryOp::CodegenLOrBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr)
:
    CodegenBinaryOp(cg, leftExpr, rightExpr, "or", "logical or")
{
    WC_EMPTY_FUNC_BODY();
}
    
void CodegenLOrBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(mCG.mCtx.mIRBuilder.CreateOr(mLeftVal.mLLVMVal,
                                              mRightVal.mLLVMVal,
                                              "Bool:LOr:Result"));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
