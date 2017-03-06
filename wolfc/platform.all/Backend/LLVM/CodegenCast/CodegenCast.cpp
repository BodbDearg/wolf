//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenCast.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
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
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Invalid)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Nullptr)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Ptr)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(Str)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt128)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt16)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt32)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt64)
    IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(UInt8)
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
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: <invalid>
//-----------------------------------------------------------------------------
class CodegenCastFromInvalidDataType final : public CodegenCastFromDataType {
public:
    CodegenCastFromInvalidDataType(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromDataType(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
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
    
    #warning TODO: nullptr: allow cast to bool
    virtual void visit(const PtrDataType & dataType) override {
        // Just do a simple pointer cast
        WC_UNUSED_PARAM(dataType);
        llvm::Constant * llvmValCast = llvm::Constant::getNullValue(mToTypeCDT.getLLVMType());
        WC_ASSERT(llvmValCast);
        mCG.mCtx.pushValue(Value(llvmValCast, mToTypeCDT, false, mCG.mCtx.getCurrentASTNode()));
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
    
    #warning TODO: nullptr: allow cast to bool
    virtual void visit(const PtrDataType & dataType) override {
        // Just do a simple pointer cast
        WC_UNUSED_PARAM(dataType);
        llvm::Value * llvmValCast = mCG.mCtx.mIRBuilder.CreatePointerCast(mFromVal.mLLVMVal, mToTypeCDT.getLLVMType());
        WC_ASSERT(llvmValCast);
        mCG.mCtx.pushValue(Value(llvmValCast, mToTypeCDT, false, mCG.mCtx.getCurrentASTNode()));
    }
    
    WC_IMPL_BASIC_CAST(CreatePtrToInt, Bool)
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
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
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
IMPL_CAST_FROM_TYPE_FUNC(Invalid)
IMPL_CAST_FROM_TYPE_FUNC(Nullptr)
IMPL_CAST_FROM_TYPE_FUNC(Ptr)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(UInt128)
IMPL_CAST_FROM_TYPE_FUNC(UInt16)
IMPL_CAST_FROM_TYPE_FUNC(UInt32)
IMPL_CAST_FROM_TYPE_FUNC(UInt64)
IMPL_CAST_FROM_TYPE_FUNC(UInt8)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
