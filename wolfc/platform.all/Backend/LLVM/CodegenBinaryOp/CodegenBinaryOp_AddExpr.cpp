#include "CodegenBinaryOp_AddExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenAddBinaryOp
//-----------------------------------------------------------------------------
CodegenAddBinaryOp::CodegenAddBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "+",
                    "add",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int128, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int16, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int32, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int64, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int8, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt128, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt16, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt32, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt64, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt8, CreateAdd)

//-----------------------------------------------------------------------------
// CodegenSubBinaryOp
//-----------------------------------------------------------------------------
CodegenSubBinaryOp::CodegenSubBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "-",
                    "subtract",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int128, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int16, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int32, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int64, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int8, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt128, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt16, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt32, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt64, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt8, CreateSub)

//-----------------------------------------------------------------------------
// CodegenBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBOrBinaryOp::CodegenBOrBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "|",
                    "bitwise or",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int128, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int16, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int32, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int64, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int8, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt128, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt16, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt32, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt64, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt8, CreateOr)

//-----------------------------------------------------------------------------
// CodegenBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBXOrBinaryOp::CodegenBXOrBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr,
                                         bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "^",
                    "bitwise xor",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int128, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int16, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int32, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int64, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int8, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt128, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt16, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt32, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt64, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt8, CreateXor)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
