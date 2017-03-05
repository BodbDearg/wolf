//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenUnaryOp_PostfixExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenIncUnaryOp
//-----------------------------------------------------------------------------
#warning Impl Ptr Arithmetic: Inc
CodegenIncUnaryOp::CodegenIncUnaryOp(Codegen & cg,
                                     const AST::ASTNode & expr,
                                     bool storeResultInExpr)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "++",
                        "increment",
                        storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

#define WC_IMPL_INC_UNARY_OP(DataTypeName, LLVMType)\
    void CodegenIncUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        llvm::ConstantInt * incByConstant = llvm::ConstantInt::get(\
            llvm::Type::get##LLVMType##Ty(mCG.mCtx.mLLVMCtx),\
            1\
        );\
        \
        WC_ASSERT(incByConstant);\
        llvm::Value * addResultVal = mCG.mCtx.mIRBuilder.CreateAdd(\
            mExprVal.mLLVMVal,\
            incByConstant,\
            #DataTypeName ":IncResult"\
        );\
        \
        WC_ASSERT(addResultVal);\
        pushOpResult(addResultVal);\
    }

WC_IMPL_INC_UNARY_OP(Int128, Int128)
WC_IMPL_INC_UNARY_OP(Int16, Int16)
WC_IMPL_INC_UNARY_OP(Int32, Int32)
WC_IMPL_INC_UNARY_OP(Int64, Int64)
WC_IMPL_INC_UNARY_OP(Int8, Int8)
WC_IMPL_INC_UNARY_OP(UInt128, Int128)
WC_IMPL_INC_UNARY_OP(UInt16, Int16)
WC_IMPL_INC_UNARY_OP(UInt32, Int32)
WC_IMPL_INC_UNARY_OP(UInt64, Int64)
WC_IMPL_INC_UNARY_OP(UInt8, Int8)

#undef WC_IMPL_INC_UNARY_OP

//-----------------------------------------------------------------------------
// CodegenDecUnaryOp
//----------------------------------------------------------------------------
#warning Impl Ptr Arithmetic: Dec
CodegenDecUnaryOp::CodegenDecUnaryOp(Codegen & cg,
                                     const AST::ASTNode & expr,
                                     bool storeResultInExpr)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "--",
                        "decrement",
                        storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

#define WC_IMPL_DEC_UNARY_OP(DataTypeName, LLVMType)\
    void CodegenDecUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        llvm::ConstantInt * decByConstant = llvm::ConstantInt::get(\
            llvm::Type::get##LLVMType##Ty(mCG.mCtx.mLLVMCtx), 1\
        );\
        \
        WC_ASSERT(decByConstant);\
        llvm::Value * subResultVal = mCG.mCtx.mIRBuilder.CreateSub(\
            mExprVal.mLLVMVal,\
            decByConstant,\
            #DataTypeName ":DecResult"\
        );\
        \
        WC_ASSERT(subResultVal);\
        pushOpResult(subResultVal);\
    }

WC_IMPL_DEC_UNARY_OP(Int128, Int128)
WC_IMPL_DEC_UNARY_OP(Int16, Int16)
WC_IMPL_DEC_UNARY_OP(Int32, Int32)
WC_IMPL_DEC_UNARY_OP(Int64, Int64)
WC_IMPL_DEC_UNARY_OP(Int8, Int8)
WC_IMPL_DEC_UNARY_OP(UInt128, Int128)
WC_IMPL_DEC_UNARY_OP(UInt16, Int16)
WC_IMPL_DEC_UNARY_OP(UInt32, Int32)
WC_IMPL_DEC_UNARY_OP(UInt64, Int64)
WC_IMPL_DEC_UNARY_OP(UInt8, Int8)

#undef WC_IMPL_DEC_UNARY_OP
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
