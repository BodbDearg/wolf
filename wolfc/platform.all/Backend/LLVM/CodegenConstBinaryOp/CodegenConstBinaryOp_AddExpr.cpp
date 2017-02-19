#include "CodegenConstBinaryOp_AddExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstAddBinaryOp
//-----------------------------------------------------------------------------
CodegenConstAddBinaryOp::CodegenConstAddBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "+", "add")
{
    WC_EMPTY_FUNC_BODY();
}


WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int128, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int16, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int32, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int64, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int8, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt128, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt16, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt32, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt64, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt8, getAdd)

//-----------------------------------------------------------------------------
// CodegenConstSubBinaryOp
//-----------------------------------------------------------------------------
CodegenConstSubBinaryOp::CodegenConstSubBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "-", "subtract")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int128, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int16, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int32, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int64, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int8, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt128, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt16, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt32, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt64, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt8, getSub)

//-----------------------------------------------------------------------------
// CodegenConstBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBOrBinaryOp::CodegenConstBOrBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "|", "bitwise or")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int128, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int16, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int32, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int64, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int8, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt128, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt16, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt32, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt64, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt8, getOr)

//-----------------------------------------------------------------------------
// CodegenConstBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBXOrBinaryOp::CodegenConstBXOrBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "^", "bitwise xor")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int128, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int16, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int32, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int64, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int8, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt128, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt16, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt32, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt64, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt8, getXor)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
