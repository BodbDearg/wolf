//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenUnaryOp_PostfixExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenIncOrDecUnaryOp
//-----------------------------------------------------------------------------
CodegenIncOrDecUnaryOp::CodegenIncOrDecUnaryOp(Codegen & cg,
                                               const AST::ASTNode & expr,
                                               const char * opSymbol,
                                               const char * opName)
:
    CodegenBasicUnaryOp(cg, expr, opSymbol, opName, true)
{
    WC_EMPTY_FUNC_BODY();
}

bool CodegenIncOrDecUnaryOp::verifyPtrTypeOkForArithmetic(const PtrDataType & ptrType) {
    if (!ptrType.isValid() || !ptrType.isSized()) {
        mCG.mCtx.error("Can't perform '%s' (%s) pointer arithmetic op on pointers of type '%s'! "
                       "The pointed to type must be valid and have a size that is known at compile "
                       "time in order for pointer arithmetic to be performed!",
                       mOpSymbol,
                       mOpName,
                       ptrType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// CodegenIncUnaryOp
//-----------------------------------------------------------------------------
CodegenIncUnaryOp::CodegenIncUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
    CodegenIncOrDecUnaryOp(cg, expr, "++", "increment")
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

void CodegenIncUnaryOp::visit(const PtrDataType & dataType) {
    // Make sure the pointer type is okay for this:
    WC_GUARD(verifyPtrTypeOkForArithmetic(dataType));
    
    // TODO: Use 32-bit type on 32 bit systems here?
    llvm::Constant * oneConst = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCG.mCtx.mLLVMCtx), 1);
    WC_ASSERT(oneConst);
    
    // Compute the new value for the pointer
    llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreateGEP(
        mExprVal.mLLVMVal,
        oneConst,
        "CodegenIncUnaryOp:Result"
    );
    
    WC_ASSERT(resultVal);
    pushOpResult(resultVal);
}

//-----------------------------------------------------------------------------
// CodegenDecUnaryOp
//----------------------------------------------------------------------------
CodegenDecUnaryOp::CodegenDecUnaryOp(Codegen & cg, const AST::ASTNode & expr) :
    CodegenIncOrDecUnaryOp(cg, expr, "--", "decrement")
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

void CodegenDecUnaryOp::visit(const PtrDataType & dataType) {
    // Make sure the pointer type is okay for this:
    WC_GUARD(verifyPtrTypeOkForArithmetic(dataType));
    
    // TODO: Use 32-bit type on 32 bit systems here?
    llvm::Constant * oneConst = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCG.mCtx.mLLVMCtx), 1);
    WC_ASSERT(oneConst);
    llvm::Constant * minusOneConst = llvm::ConstantExpr::getNeg(oneConst);
    WC_ASSERT(minusOneConst);
    
    // Compute the new value for the pointer
    llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreateGEP(
        mExprVal.mLLVMVal,
        minusOneConst,
        "CodegenDecUnaryOp:Result"
    );
    
    WC_ASSERT(resultVal);
    pushOpResult(resultVal);
}
    
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
