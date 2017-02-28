//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstBinaryOp_MulExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstMulBinaryOp
//-----------------------------------------------------------------------------
CodegenConstMulBinaryOp::CodegenConstMulBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "*", "multiply")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, Int128, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, Int16, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, Int32, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, Int64, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, Int8, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, UInt128, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, UInt16, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, UInt32, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, UInt64, getMul)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstMulBinaryOp, UInt8, getMul)

//-----------------------------------------------------------------------------
// CodegenConstDivBinaryOp
//-----------------------------------------------------------------------------
CodegenConstDivBinaryOp::CodegenConstDivBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "/", "divide")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, Int128, getSDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, Int16, getSDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, Int32, getSDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, Int64, getSDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, Int8, getSDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, UInt128, getUDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, UInt16, getUDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, UInt32, getUDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, UInt64, getUDiv)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstDivBinaryOp, UInt8, getUDiv)

//-----------------------------------------------------------------------------
// CodegenConstRemBinaryOp
//-----------------------------------------------------------------------------
CodegenConstRemBinaryOp::CodegenConstRemBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "%", "remainder")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, Int128, getSRem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, Int64, getSRem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, Int32, getSRem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, Int16, getSRem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, Int8, getSRem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, UInt128, getURem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, UInt64, getURem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, UInt32, getURem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, UInt16, getURem)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstRemBinaryOp, UInt8, getURem)

//-----------------------------------------------------------------------------
// CodegenConstBAndBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBAndBinaryOp::CodegenConstBAndBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "&", "bitwise and")
{
    WC_EMPTY_FUNC_BODY();
}
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, Int128, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, Int16, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, Int32, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, Int64, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, Int8, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, UInt128, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, UInt16, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, UInt32, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, UInt64, getAnd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBAndBinaryOp, UInt8, getAnd)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
