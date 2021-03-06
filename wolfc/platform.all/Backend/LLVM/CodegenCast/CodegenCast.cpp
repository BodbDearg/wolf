//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenCast.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Abstract base class that converts from one type at runtime to many other
// types at runtime.
//-----------------------------------------------------------------------------
class CodegenCastFromDataType : public DataTypeVisitor {
public:
    CodegenCastFromDataType(Codegen & cg,
                            const Value & fromVal,
                            const CompiledDataType & toTypeCDT)
    :
        mCG(cg),
        mFromVal(fromVal),
        mToTypeCDT(toTypeCDT)
    {
        WC_ASSERT(mFromVal.isValid());
        WC_ASSERT(mToTypeCDT.isValid());
    }
    
    /* This saves a lot of repetition */
    #define IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(DataTypeName)\
        virtual void visit(const DataTypeName##DataType & dataType) override {\
            WC_UNUSED_PARAM(dataType);\
            issueUnsupportedCastError();\
        }
    
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Any)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Array)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Bool)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Func)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Int128)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Int16)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Int32)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Int64)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Int8)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Nullptr)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Ptr)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Str)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt128)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt16)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt32)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt64)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt8)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Undefined)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Void)
    
    /* Done with this now! */
    #undef IMPL_UNSUPPORTED_CAST_TO_TYPE_FUNC
    
    /* Save the operator result in the codegen context */
    void pushOpResult(llvm::Value * result) {
        const AST::ASTNode * declaringNode = nullptr;
        const auto & astNodeStack = mCG.mCtx.getASTNodeStack();
        
        if (astNodeStack.empty()) {
            declaringNode = astNodeStack.back();
        }
        
        mCG.mCtx.pushValue(Value(result, mToTypeCDT, false, declaringNode));
    }
    
    /* The codegen function */
    void codegen() {
        mToTypeCDT.getDataType().accept(*this);
    }
    
protected:
    /* Spit out an error that a cast is not supported */
    void issueUnsupportedCastError() const {
        mCG.mCtx.error("Illegal cast! Cannot cast from an expression of type '%s' to type '%s'!",
                       mFromVal.mCompiledType.getDataType().name().c_str(),
                       mToTypeCDT.getDataType().name().c_str());
    }    
    
    Codegen &                   mCG;
    const Value &               mFromVal;
    const CompiledDataType &    mToTypeCDT;
};

/* This macro saves a lot of repetition */
#define WC_IMPL_BASIC_CAST(CastCreateFunc, ToDataTypeName)\
    virtual void visit(const ToDataTypeName##DataType & toType) override {\
        WC_UNUSED_PARAM(toType);\
        WC_ASSERT(mFromVal.mLLVMVal);\
        WC_ASSERT(mToTypeCDT.getLLVMType());\
        pushOpResult(mCG.mCtx.mIRBuilder.CastCreateFunc(mFromVal.mLLVMVal, mToTypeCDT.getLLVMType()));\
    }

//-----------------------------------------------------------------------------
// Cast From: any
//-----------------------------------------------------------------------------
class CodegenCastFromAnyDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromAnyDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Array
//-----------------------------------------------------------------------------
class CodegenCastFromArrayDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromArrayDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: bool
//-----------------------------------------------------------------------------
class CodegenCastFromBoolDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromBoolDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Func
//-----------------------------------------------------------------------------
class CodegenCastFromFuncDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromFuncDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: int128
//-----------------------------------------------------------------------------
class CodegenCastFromInt128DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInt128DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateBitCast, Int128)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt128)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int16
//-----------------------------------------------------------------------------
class CodegenCastFromInt16DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInt16DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateSExt, Int32)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int32
//-----------------------------------------------------------------------------
class CodegenCastFromInt32DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInt32DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int64
//-----------------------------------------------------------------------------
class CodegenCastFromInt64DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInt64DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int8
//-----------------------------------------------------------------------------
class CodegenCastFromInt8DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInt8DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateSExt, Int32)
    WC_IMPL_BASIC_CAST(CreateSExt, Int16)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: nullptr
//-----------------------------------------------------------------------------
class CodegenCastFromNullptrDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromNullptrDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int8)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int16)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int32)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int64)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int128)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt8)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt16)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt32)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt64)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt128)
    
    virtual void visit(const BoolDataType & dataType) override {
        // 'null' will always equate to 'false' so just fastrack that comparison here
        WC_UNUSED_PARAM(dataType);
        llvm::Constant * falseConst = llvm::ConstantInt::get(llvm::Type::getInt1Ty(mCG.mCtx.mLLVMCtx), 0);
        WC_ASSERT(falseConst);
        mCG.mCtx.pushValue(Value(falseConst, mToTypeCDT, false, mCG.mCtx.getCurrentASTNode()));
    }
    
    virtual void visit(const PtrDataType & toType) override {
        // Expect this to be valid
        WC_ASSERT(mFromVal.mLLVMVal);
        
        // Only allow a cast to the pointer data type if the pointer type is not null.
        // Can't cast 'null' to a non nullable pointer type.
        if (toType.mIsNullable) {
            pushOpResult(mCG.mCtx.mIRBuilder.CreatePointerCast(mFromVal.mLLVMVal, mToTypeCDT.getLLVMType()));
        }
        else {
            mCG.mCtx.error("Casting the 'null' literal to a non nullable pointer of type '%s' is illegal! "
                           "Non nullable pointers are not allowed to be null in well defined code.",
                           toType.mName.c_str());
        }
    }
};

//-----------------------------------------------------------------------------
// Cast From: Ptr
//-----------------------------------------------------------------------------
class CodegenCastFromPtrDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromPtrDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int8)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int16)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int32)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int64)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Int128)
    WC_IMPL_BASIC_CAST(CreatePointerCast, Ptr)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt8)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt16)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt32)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt64)
    WC_IMPL_BASIC_CAST(CreatePtrToInt, UInt128)

    virtual void visit(const BoolDataType & dataType) override {
        // Expect this to be valid
        WC_ASSERT(mFromVal.mLLVMVal);
        
        // Doing this for a non nullable pointer type is an error:
        const PtrDataType & fromPtrType = static_cast<const PtrDataType&>(mFromVal.mCompiledType.getDataType());
        
        if (!fromPtrType.mIsNullable) {
            mCG.mCtx.error("Attempting to cast a non nullable pointer of type '%s' to type '%s' via a pointer "
                           "null check! Non nullable pointers can never be null in well defined code, hence "
                           "checking if they are null and converting to bool based on that result is illegal!",
                           fromPtrType.name().c_str(),
                           dataType.name().c_str());
            
            return;
        }
        
        // Compare the pointer against 'null'
        llvm::Constant * nullConst = llvm::Constant::getNullValue(mFromVal.mCompiledType.getLLVMType());
        WC_ASSERT(nullConst);
        llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreateICmpNE(mFromVal.mLLVMVal, nullConst);
        mCG.mCtx.pushValue(Value(resultVal, mToTypeCDT, false, mCG.mCtx.getCurrentASTNode()));
    }
};

//-----------------------------------------------------------------------------
// Cast From: str
//-----------------------------------------------------------------------------
class CodegenCastFromStrDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromStrDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: uint128
//-----------------------------------------------------------------------------
class CodegenCastFromUInt128DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUInt128DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateBitCast, Int128)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt128)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint16
//-----------------------------------------------------------------------------
class CodegenCastFromUInt16DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUInt16DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateZExt, Int32)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint32
//-----------------------------------------------------------------------------
class CodegenCastFromUInt32DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUInt32DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint64
//-----------------------------------------------------------------------------
class CodegenCastFromUInt64DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUInt64DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint8
//-----------------------------------------------------------------------------
class CodegenCastFromUInt8DataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUInt8DataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateZExt, Int32)
    WC_IMPL_BASIC_CAST(CreateZExt, Int16)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int8)
    WC_IMPL_BASIC_CAST(CreateIntToPtr, Ptr)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: <undefined>
//-----------------------------------------------------------------------------
class CodegenCastFromUndefinedDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromUndefinedDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: void
//-----------------------------------------------------------------------------
class CodegenCastFromVoidDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromVoidDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// CodegenCast
//-----------------------------------------------------------------------------
CodegenCast::CodegenCast(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
    mCG(cg),
    mFromVal(fromVal),
    mToTypeCDT(toTypeCDT)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCast::codegen() {
    // If the value we are casting is invalid then bail:
    if (!mFromVal.isValid()) {
        mCG.mCtx.error("Can't code generate a cast from an invalid value of type '%s'!",
                       mFromVal.mCompiledType.getDataType().name().c_str());
        
        return;
    }
    
    // If the type we are casting to is invalid then bail:
    if (!mToTypeCDT.isValid()) {
        mCG.mCtx.error("Can't code generate a cast to invalid data type '%s'!",
                       mToTypeCDT.getDataType().name().c_str());
        
        return;
    }
    
    // If the data types of the source and target expression match then do nothing - no cast required.
    // In this case just push the value back onto the codegen context to finish up.
    if (mToTypeCDT.getDataType().equals(mFromVal.mCompiledType.getDataType())) {
        mCG.mCtx.pushValue(mFromVal);
        return;
    }
    
    // Otherwise codegen the cast:
    mFromVal.mCompiledType.getDataType().accept(*this);
}

/* Macro to save a lot of repetition */
#define IMPL_CAST_FROM_TYPE_FUNC(FromTypeName)\
    void CodegenCast::visit(const FromTypeName##DataType & dataType) {\
        /* We don't use the input to these functions */\
        WC_UNUSED_PARAM(dataType);\
        /* Sanity check, we shouldn't get to here if these conditions are not true! */\
        WC_ASSERT(mFromVal.isValid());\
        WC_ASSERT(mToTypeCDT.isValid());\
        \
        /* Okay, create the helper object which will do the cast and invoke it */\
        CodegenCastFrom##FromTypeName##DataType(mCG,\
                                                mFromVal,\
                                                mToTypeCDT).codegen();\
    }

IMPL_CAST_FROM_TYPE_FUNC(Any)
IMPL_CAST_FROM_TYPE_FUNC(Array)
IMPL_CAST_FROM_TYPE_FUNC(Bool)
IMPL_CAST_FROM_TYPE_FUNC(Func)
IMPL_CAST_FROM_TYPE_FUNC(Int128)
IMPL_CAST_FROM_TYPE_FUNC(Int16)
IMPL_CAST_FROM_TYPE_FUNC(Int32)
IMPL_CAST_FROM_TYPE_FUNC(Int64)
IMPL_CAST_FROM_TYPE_FUNC(Int8)
IMPL_CAST_FROM_TYPE_FUNC(Nullptr)
IMPL_CAST_FROM_TYPE_FUNC(Ptr)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(UInt128)
IMPL_CAST_FROM_TYPE_FUNC(UInt16)
IMPL_CAST_FROM_TYPE_FUNC(UInt32)
IMPL_CAST_FROM_TYPE_FUNC(UInt64)
IMPL_CAST_FROM_TYPE_FUNC(UInt8)
IMPL_CAST_FROM_TYPE_FUNC(Undefined)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
