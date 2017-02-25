#include "CodegenBinaryOp_MulExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenMulBinaryOp
//-----------------------------------------------------------------------------
CodegenMulBinaryOp::CodegenMulBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr, 
                    "*",
                    "multiply",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, Int128, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, Int16, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, Int32, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, Int64, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, Int8, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, UInt128, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, UInt16, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, UInt32, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, UInt64, CreateMul)
WC_IMPL_BASIC_BINARY_OP(CodegenMulBinaryOp, UInt8, CreateMul)

//-----------------------------------------------------------------------------
// CodegenDivBinaryOp
//-----------------------------------------------------------------------------
CodegenDivBinaryOp::CodegenDivBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "/",
                    "divide",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, Int128, CreateSDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, Int16, CreateSDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, Int32, CreateSDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, Int64, CreateSDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, Int8, CreateSDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, UInt128, CreateUDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, UInt16, CreateUDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, UInt32, CreateUDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, UInt64, CreateUDiv)
WC_IMPL_BASIC_BINARY_OP(CodegenDivBinaryOp, UInt8, CreateUDiv)

//-----------------------------------------------------------------------------
// CodegenRemBinaryOp
//-----------------------------------------------------------------------------
CodegenRemBinaryOp::CodegenRemBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "%",
                    "remainder",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, Int128, CreateSRem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, Int64, CreateSRem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, Int32, CreateSRem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, Int16, CreateSRem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, Int8, CreateSRem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, UInt128, CreateURem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, UInt64, CreateURem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, UInt32, CreateURem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, UInt16, CreateURem)
WC_IMPL_BASIC_BINARY_OP(CodegenRemBinaryOp, UInt8, CreateURem)

//-----------------------------------------------------------------------------
// CodegenBAndBinaryOp
//-----------------------------------------------------------------------------
CodegenBAndBinaryOp::CodegenBAndBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr,
                                         bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "&",
                    "bitwise and",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, Int128, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, Int16, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, Int32, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, Int64, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, Int8, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, UInt128, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, UInt16, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, UInt32, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, UInt64, CreateAnd)
WC_IMPL_BASIC_BINARY_OP(CodegenBAndBinaryOp, UInt8, CreateAnd)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
