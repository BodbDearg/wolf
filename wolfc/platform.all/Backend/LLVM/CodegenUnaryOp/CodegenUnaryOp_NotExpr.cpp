//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

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
    CodegenBasicUnaryOp(cg,
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
    CodegenBasicUnaryOp(cg,
                        expr,
                        "~",
                        "bitwise not",
                        storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, Int128, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, Int16, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, Int32, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, Int64, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, Int8, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, UInt128, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, UInt16, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, UInt32, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, UInt64, CreateNot)
WC_IMPL_BASIC_UNARY_OP(CodegenBNotUnaryOp, UInt8, CreateNot)
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
