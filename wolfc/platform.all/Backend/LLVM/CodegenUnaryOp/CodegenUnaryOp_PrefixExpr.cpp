#include "CodegenUnaryOp_PrefixExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenPlusUnaryOp::CodegenPlusUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "+",
                   "plus",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

#define WC_IMPL_PLUS_UNARY_OP(DataTypeName)\
    void CodegenPlusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        pushOpResult(mExprVal.mLLVMVal);\
    }

WC_IMPL_PLUS_UNARY_OP(Int128)
WC_IMPL_PLUS_UNARY_OP(Int16)
WC_IMPL_PLUS_UNARY_OP(Int32)
WC_IMPL_PLUS_UNARY_OP(Int64)
WC_IMPL_PLUS_UNARY_OP(Int8)
WC_IMPL_PLUS_UNARY_OP(UInt128)
WC_IMPL_PLUS_UNARY_OP(UInt16)
WC_IMPL_PLUS_UNARY_OP(UInt32)
WC_IMPL_PLUS_UNARY_OP(UInt64)
WC_IMPL_PLUS_UNARY_OP(UInt8)

#undef WC_IMPL_PLUS_UNARY_OP

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//----------------------------------------------------------------------------
CodegenMinusUnaryOp::CodegenMinusUnaryOp(Codegen & cg,
                                         const AST::ASTNode & expr,
                                         bool storeResultInExpr)
:
    CodegenUnaryOp(cg,
                   expr,
                   "-",
                   "minus",
                   storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int128, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int16, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int32, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int64, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int8, CreateNeg)

#define WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(DataTypeName)\
    void CodegenMinusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        mCG.mCtx.error("Negation operator (-) is not allowed for an expression of unsigned "\
                       "data type '%s'!",\
                       mExprVal.mCompiledType.getDataType().name().c_str());\
    }

WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt128)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt16)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt32)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt64)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt8)

#undef WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
