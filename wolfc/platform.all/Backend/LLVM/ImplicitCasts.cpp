//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ImplicitCasts.hpp"

#include "Codegen/Codegen.hpp"
#include "CodegenCast/CodegenCast.hpp"
#include "CodegenConstCast/CodegenConstCast.hpp"
#include "CodegenCtx.hpp"
#include "ConstCodegen/ConstCodegen.hpp"
#include "DataType/DataType.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE
WC_BEGIN_NAMED_NAMESPACE(ImplicitCasts)

//------------------------------------------------------------------------------
// Base class to check if a cast is allowed from a particular type to any
// number of other types.
//------------------------------------------------------------------------------
class CheckCastAllowedFromType : public DataTypeVisitor {
public:
    CheckCastAllowedFromType(const CompiledDataType & fromType,
                             const CompiledDataType & toType,
                             llvm::Constant * fromConst)
    :
        mFromType(fromType),
        mToType(toType),
        mFromConst(fromConst),
        mCastAllowed(false)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* Check to see if the cast is allowed */
    bool check() {
        mCastAllowed = false;
        mToType.getDataType().accept(*this);
        return mCastAllowed;
    }
    
    /* This saves a lot of repetition */
    #define IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC(DataTypeName)\
        virtual void visit(const DataTypeName##DataType & dataType) override {\
            WC_UNUSED_PARAM(dataType);\
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
    
    #undef IMPL_CAST_TO_TYPE_UNSUPPORTED_FUNC

protected:
    const CompiledDataType &    mFromType;
    const CompiledDataType &    mToType;
    llvm::Constant *            mFromConst;
    bool                        mCastAllowed;
};

/* Useful macro */
#define IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(DataTypeName)\
    virtual void visit(const DataTypeName##DataType & dataType) override {\
        WC_UNUSED_PARAM(dataType);\
        mCastAllowed = true;\
    }

//------------------------------------------------------------------------------
// Check cast allowed from: any
//------------------------------------------------------------------------------
class CheckCastAllowedFromAny final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromAny(const CompiledDataType & fromType,
                            const CompiledDataType & toType,
                            llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: Array
//------------------------------------------------------------------------------
class CheckCastAllowedFromArray final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromArray(const CompiledDataType & fromType,
                              const CompiledDataType & toType,
                              llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: bool
//------------------------------------------------------------------------------
class CheckCastAllowedFromBool final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromBool(const CompiledDataType & fromType,
                             const CompiledDataType & toType,
                             llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: Func
//------------------------------------------------------------------------------
class CheckCastAllowedFromFunc final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromFunc(const CompiledDataType & fromType,
                             const CompiledDataType & toType,
                             llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: int128
//------------------------------------------------------------------------------
class CheckCastAllowedFromInt128 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInt128(const CompiledDataType & fromType,
                               const CompiledDataType & toType,
                               llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: int16
//------------------------------------------------------------------------------
class CheckCastAllowedFromInt16 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInt16(const CompiledDataType & fromType,
                              const CompiledDataType & toType,
                              llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: int32
//------------------------------------------------------------------------------
class CheckCastAllowedFromInt32 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInt32(const CompiledDataType & fromType,
                              const CompiledDataType & toType,
                              llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: int64
//------------------------------------------------------------------------------
class CheckCastAllowedFromInt64 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInt64(const CompiledDataType & fromType,
                              const CompiledDataType & toType,
                              llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: int8
//------------------------------------------------------------------------------
class CheckCastAllowedFromInt8 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInt8(const CompiledDataType & fromType,
                             const CompiledDataType & toType,
                             llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int16)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: <invalid>
//------------------------------------------------------------------------------
class CheckCastAllowedFromInvalid final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromInvalid(const CompiledDataType & fromType,
                                const CompiledDataType & toType,
                                llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: nullptr
//------------------------------------------------------------------------------
class CheckCastAllowedFromNullptr final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromNullptr(const CompiledDataType & fromType,
                                const CompiledDataType & toType,
                                llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    virtual void visit(const PtrDataType & dataType) override {
        // Cast to a pointer type is allowe if the pointer is nullable
        WC_UNUSED_PARAM(dataType);
        mCastAllowed = dataType.mIsNullable;
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: Ptr
//------------------------------------------------------------------------------
class CheckCastAllowedFromPtr final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromPtr(const CompiledDataType & fromType,
                            const CompiledDataType & toType,
                            llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    virtual void visit(const PtrDataType & dataType) override {
        // Implicit cast for pointers is only allowed if going from a non nullable pointer of
        // one type to a nullable pointer of the same type...
        //
        // TODO: Some day we should support implicit casts for structs, classes etc. going from
        // derived types back to base types.
        WC_UNUSED_PARAM(dataType);
        const PtrDataType & fromType = static_cast<const PtrDataType&>(mFromType.getDataType());
        const PtrDataType & toType = static_cast<const PtrDataType&>(mToType.getDataType());
        
        if (!fromType.mIsNullable && toType.mIsNullable) {
            if (fromType.mPointedToType.equals(toType.mPointedToType)) {
                mCastAllowed = true;
            }
        }
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: str
//------------------------------------------------------------------------------
class CheckCastAllowedFromStr final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromStr(const CompiledDataType & fromType,
                            const CompiledDataType & toType,
                            llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: uint128
//------------------------------------------------------------------------------
class CheckCastAllowedFromUInt128 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUInt128(const CompiledDataType & fromType,
                                const CompiledDataType & toType,
                                llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check cast allowed from: uint16
//------------------------------------------------------------------------------
class CheckCastAllowedFromUInt16 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUInt16(const CompiledDataType & fromType,
                               const CompiledDataType & toType,
                               llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: uint32
//------------------------------------------------------------------------------
class CheckCastAllowedFromUInt32 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUInt32(const CompiledDataType & fromType,
                               const CompiledDataType & toType,
                               llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: uint64
//------------------------------------------------------------------------------
class CheckCastAllowedFromUInt64 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUInt64(const CompiledDataType & fromType,
                               const CompiledDataType & toType,
                               llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: uint8
//------------------------------------------------------------------------------
class CheckCastAllowedFromUInt8 final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUInt8(const CompiledDataType & fromType,
                              const CompiledDataType & toType,
                              llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int16)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Int128)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt16)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt32)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt64)
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(UInt128)
};

//------------------------------------------------------------------------------
// Check cast allowed from: void
//------------------------------------------------------------------------------
class CheckCastAllowedFromVoid final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromVoid(const CompiledDataType & fromType,
                             const CompiledDataType & toType,
                             llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//------------------------------------------------------------------------------
// Check if an implicit cast is allowed from the given type to the given type.
// A constant value can also be used (if the value is a constant) to enable
// additional implicit casts based on the value. For example an 'int64' value of
// '1' could be allowed to convert to an 'int8' because no data loss would occur.
//------------------------------------------------------------------------------
class CheckCastAllowed final : public DataTypeVisitor {
public:
    CheckCastAllowed(const CompiledDataType & fromType,
                     const CompiledDataType & toType,
                     llvm::Constant * fromConst)
    :
        mFromType(fromType),
        mToType(toType),
        mFromConst(fromConst),
        mCastAllowed(false)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* Check to see if the cast is allowed */
    bool check() {
        mFromType.getDataType().accept(*this);
        return mCastAllowed;
    }
    
    /* This saves a lot of repetition */
    #define IMPL_CAST_FROM_TYPE_FUNC(DataTypeName)\
        virtual void visit(const DataTypeName##DataType & dataType) override {\
            WC_UNUSED_PARAM(dataType);\
            CheckCastAllowedFrom##DataTypeName castCheck(mFromType, mToType, mFromConst);\
            mCastAllowed = castCheck.check();\
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
    
private:
    const CompiledDataType &    mFromType;
    const CompiledDataType &    mToType;
    llvm::Constant *            mFromConst;
    bool                        mCastAllowed;
};

bool canDoImplicitCast(const CompiledDataType & fromType,
                       const CompiledDataType & toType,
                       llvm::Value * fromVal)
{
    CheckCastAllowed castCheck(fromType, toType, llvm::dyn_cast_or_null<llvm::Constant>(fromVal));
    return castCheck.check();
}

Value castSingleValueIfRequired(Codegen & cg,
                                const Value & value,
                                const CompiledDataType & toTypeCDT)
{
    // If the data type of the value equals the type given then no cast is required.
    // In this case return the value given:
    const CompiledDataType & valueCDT = value.mCompiledType;
    WC_GUARD(!valueCDT.getDataType().equals(toTypeCDT.getDataType()), value);
    
    // If the implicit cast is not allowed then do not proceed.
    // In this case also return the value given:
    WC_GUARD(canDoImplicitCast(valueCDT, toTypeCDT, nullptr), value);
    
    // Otherwise do the cast and return the result:
    CodegenCast(cg, value, toTypeCDT).codegen();
    return cg.mCtx.popValue();
}

Constant castSingleConstantIfRequired(ConstCodegen & cg,
                                      const Constant & constant,
                                      const CompiledDataType & toTypeCDT)
{
    // If the data type of the value equals the type given then no cast is required.
    // In this case return the value given:
    const CompiledDataType & constantCDT = constant.mCompiledType;
    WC_GUARD(!constantCDT.getDataType().equals(toTypeCDT.getDataType()), constant);
    
    // If the implicit cast is not allowed then do not proceed.
    // In this case also return the value given:
    WC_GUARD(canDoImplicitCast(constantCDT, toTypeCDT, constant.mLLVMConst), constant);
    
    // Otherwise do the cast and return the result:
    CodegenConstCast(cg, constant, toTypeCDT).codegen();
    return cg.mCtx.popConstant();
}

WC_END_NAMED_NAMESPACE(ImplicitCast)
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
