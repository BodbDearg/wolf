#include "CodegenBinaryOp_ShiftExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenLShiftBinaryOp::CodegenLShiftBinaryOp(Codegen & cg,
                                             const AST::ASTNode & leftExpr,
                                             const AST::ASTNode & rightExpr,
                                             bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "<<",
                    "left shift",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, Int128, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, Int16, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, Int32, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, Int64, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, Int8, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, UInt128, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, UInt16, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, UInt32, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, UInt64, CreateShl)
WC_IMPL_BASIC_BINARY_OP(CodegenLShiftBinaryOp, UInt8, CreateShl)

//-----------------------------------------------------------------------------
// CodegenARShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenARShiftBinaryOp::CodegenARShiftBinaryOp(Codegen & cg,
                                               const AST::ASTNode & leftExpr,
                                               const AST::ASTNode & rightExpr,
                                               bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    ">>",
                    "arithmetic right shift",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, Int128, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, Int16, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, Int32, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, Int64, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, Int8, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, UInt128, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, UInt16, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, UInt32, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, UInt64, CreateAShr)
WC_IMPL_BASIC_BINARY_OP(CodegenARShiftBinaryOp, UInt8, CreateAShr)

//-----------------------------------------------------------------------------
// CodegenLRShiftBinaryOp
//-----------------------------------------------------------------------------
CodegenLRShiftBinaryOp::CodegenLRShiftBinaryOp(Codegen & cg,
                                               const AST::ASTNode & leftExpr,
                                               const AST::ASTNode & rightExpr,
                                               bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    ">>>",
                    "logical right shift",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, Int128, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, Int16, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, Int32, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, Int64, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, Int8, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, UInt128, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, UInt16, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, UInt32, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, UInt64, CreateLShr)
WC_IMPL_BASIC_BINARY_OP(CodegenLRShiftBinaryOp, UInt8, CreateLShr)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
