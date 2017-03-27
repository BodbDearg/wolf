//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstUnaryOp_PrefixExpr.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstPlusUnaryOp
//-----------------------------------------------------------------------------
CodegenConstPlusUnaryOp::CodegenConstPlusUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr)
:
    CodegenBasicConstUnaryOp(cg, expr, "+", "plus")
{
    WC_EMPTY_FUNC_BODY();
}

#define WC_IMPL_CONST_PLUS_UNARY_OP(DataTypeName)\
    void CodegenConstPlusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        WC_ASSERT(mExprConst.mLLVMConst);\
        pushOpResult(mExprConst.mLLVMConst);\
    }

WC_IMPL_CONST_PLUS_UNARY_OP(Int128)
WC_IMPL_CONST_PLUS_UNARY_OP(Int16)
WC_IMPL_CONST_PLUS_UNARY_OP(Int32)
WC_IMPL_CONST_PLUS_UNARY_OP(Int64)
WC_IMPL_CONST_PLUS_UNARY_OP(Int8)
WC_IMPL_CONST_PLUS_UNARY_OP(UInt128)
WC_IMPL_CONST_PLUS_UNARY_OP(UInt16)
WC_IMPL_CONST_PLUS_UNARY_OP(UInt32)
WC_IMPL_CONST_PLUS_UNARY_OP(UInt64)
WC_IMPL_CONST_PLUS_UNARY_OP(UInt8)

#undef WC_IMPL_CONST_PLUS_UNARY_OP

//-----------------------------------------------------------------------------
// CodegenConstMinusUnaryOp
//-----------------------------------------------------------------------------
CodegenConstMinusUnaryOp::CodegenConstMinusUnaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & expr)
:
    CodegenBasicConstUnaryOp(cg, expr, "-", "minus")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstMinusUnaryOp, Int128, getNeg)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstMinusUnaryOp, Int16, getNeg)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstMinusUnaryOp, Int32, getNeg)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstMinusUnaryOp, Int64, getNeg)
WC_IMPL_BASIC_CONST_UNARY_OP(CodegenConstMinusUnaryOp, Int8, getNeg)

#define WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(DataTypeName)\
    void CodegenConstMinusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        mCG.mCtx.error("Negation operator (-) is not allowed for an expression of unsigned "\
                       "data type '%s'!",\
                       mExprConst.mCompiledType.getDataType().name().c_str());\
    }

WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt128)
WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt16)
WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt32)
WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt64)
WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt8)

#undef WC_IMPL_CONST_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE

//-----------------------------------------------------------------------------
// CodegenConstPtrDenullUnaryOp
//-----------------------------------------------------------------------------
CodegenConstPtrDenullUnaryOp::CodegenConstPtrDenullUnaryOp(ConstCodegen & cg,
                                                           const AST::ASTNode & expr)
:
    CodegenBasicConstUnaryOp(cg, expr, "^", "pointer denull")
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstPtrDenullUnaryOp::codegen() {
    // Evaluate the operand expression. If that fails then bail out:
    mExpr.accept(mCG);
    Constant exprConst = mCG.mCtx.popConstant();
    WC_GUARD(exprConst.isValid());
    
    // The expression must be a pointer type:
    const CompiledDataType & exprCDT = exprConst.mCompiledType;
    const DataType & exprDT = exprCDT.getDataType();
    
    if (!exprDT.isPtr()) {
        mCG.mCtx.error("Can only apply the '^' (pointer denull) prefix operator to pointer types! The expression of type '%s' "
                       "being denulled is not a pointer!",
                       exprDT.name().c_str());
        
        return;
    }
    
    // The pointer type must be nullable:
    const PtrDataType & ptrDT = static_cast<const PtrDataType&>(exprDT);
    
    if (!ptrDT.mIsNullable) {
        mCG.mCtx.error("Can only apply the '^' (denull) prefix operator to nullable pointer types! The expression of type '%s' "
                       "is not a nullable pointer!",
                       ptrDT.name().c_str());
        
        return;
    }
    
    // Create the compiled data type for the result expression
    //
    // TODO: What allocates this memory?
    PtrDataType * nonNullablePtrDT = new PtrDataType(ptrDT.mPointedToType, false);
    nonNullablePtrDT->accept(mCG.mCodegenDataType);
    CompiledDataType nonNullablePtrCDT = mCG.mCtx.popCompiledDataType();
    
    // Save the result:
    mCG.mCtx.pushConstant(Constant(exprConst.mLLVMConst, nonNullablePtrCDT, &mExpr));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
