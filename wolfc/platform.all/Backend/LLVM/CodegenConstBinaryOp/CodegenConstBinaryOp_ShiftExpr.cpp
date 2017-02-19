#include "CodegenConstBinaryOp_ShiftExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLShiftBinaryOp::CodegenConstLShiftBinaryOp(ConstCodegen & cg,
                                                       const AST::ASTNode & leftExpr,
                                                       const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "<<", "left shift")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, Int128, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, Int16, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, Int32, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, Int64, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, Int8, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, UInt128, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, UInt16, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, UInt32, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, UInt64, getShl)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLShiftBinaryOp, UInt8, getShl)

//-----------------------------------------------------------------------------
// CodegenConstARShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstARShiftBinaryOp::CodegenConstARShiftBinaryOp(ConstCodegen & cg,
                                                         const AST::ASTNode & leftExpr,
                                                         const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, ">>", "arithmetic right shift")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, Int128, getAShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, Int16, getAShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, Int32, getAShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, Int64, getAShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, Int8, getAShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, UInt128, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, UInt16, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, UInt32, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, UInt64, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstARShiftBinaryOp, UInt8, getLShr)

//-----------------------------------------------------------------------------
// CodegenConstLRShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenConstLRShiftBinaryOp::CodegenConstLRShiftBinaryOp(ConstCodegen & cg,
                                                         const AST::ASTNode & leftExpr,
                                                         const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, ">>>", "logical right shift")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, Int128, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, Int16, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, Int32, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, Int64, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, Int8, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, UInt128, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, UInt16, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, UInt32, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, UInt64, getLShr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstLRShiftBinaryOp, UInt8, getLShr)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
