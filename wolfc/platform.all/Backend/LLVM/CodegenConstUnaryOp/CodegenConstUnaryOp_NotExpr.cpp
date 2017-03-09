//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstUnaryOp_NotExpr.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "../ImplicitCasts.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenConstLNotUnaryOp::CodegenConstLNotUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenBasicConstUnaryOp(cg, expr, "not", "logical not")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLNotUnaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    pushOpResult(llvm::ConstantExpr::getNot(mExprConst.mLLVMConst));
}

void CodegenConstLNotUnaryOp::doAnyImplicitTypeCastingRequiredForExprConst() {
    // The expression value must always be converted to the 'bool' type.
    // The first step is to codegen this type:
    PrimitiveDataTypes::getBoolDataType().accept(mCG.mCodegenDataType);
    CompiledDataType boolType = mCG.mCtx.popCompiledDataType();
    
    // Next, do any conversions that are required:
    ImplicitCasts::castSingleConstantIfRequired(mCG, mExprConst, boolType);
}

//-----------------------------------------------------------------------------
// CodegenConstBNotUnaryOp
//-----------------------------------------------------------------------------
CodegenConstBNotUnaryOp::CodegenConstBNotUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenBasicConstUnaryOp(cg, expr, "~", "bitwise not")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, Int128, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, Int16, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, Int32, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, Int64, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, Int8, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, UInt128, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, UInt16, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, UInt32, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, UInt64, getNot)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstBNotUnaryOp, UInt8, getNot)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
