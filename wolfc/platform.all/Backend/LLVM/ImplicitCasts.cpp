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
#include "DataType/Types/GenericIntDataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE
WC_BEGIN_NAMED_NAMESPACE(ImplicitCasts)

/**
 * Utility function, checks to see if we can implicitly cast from one integer type to another 
 * without losing any data.
 *
 * If a constant value is known for the integer, this enables additional casts that might not
 * have otherwise been possible.
 */
static bool canImplicitCastIntToIntType(const CompiledDataType & fromTypeCDT,
                                        const CompiledDataType & toTypeCDT,
                                        llvm::Constant * fromConst)
{
    // If the types are equal no implicit casting is allowed:
    const DataType & fromType = fromTypeCDT.getDataType();
    const DataType & toType = toTypeCDT.getDataType();
    WC_GUARD(!fromType.equals(toType), false);
    
    // Get the llvm integer types we are casting from and to.
    // If either types are not integers then the cast will not be possible...
    llvm::IntegerType * fromLLVMIntType = llvm::dyn_cast<llvm::IntegerType>(fromTypeCDT.getLLVMType());
    WC_GUARD(fromLLVMIntType, false);
    llvm::IntegerType * toLLVMIntType = llvm::dyn_cast<llvm::IntegerType>(toTypeCDT.getLLVMType());
    WC_GUARD(toLLVMIntType, false);
    
    // If we have no known constant integer value to play with then the rules are simple:
    //
    // 1 - Can only cast to a larger integer type.
    // 2 - Can't cast from a signed type to an unsigned type.
    if (!fromConst) {
        if (fromType.isSigned() && !toType.isSigned()) {
            // Signed check failed: can't implicitly cast signed to unsigned!
            return false;
        }
        
        if (fromLLVMIntType->getBitWidth() >= toLLVMIntType->getBitWidth()) {
            // Bigger check failed: only allow to go from a smaller int type to a larger one
            return false;
        }
        
        // If we got to here then all is well, return the value
        return true;
    }
    
    // Cast the value to a constant int and grab it's value:
    llvm::ConstantInt * fromConstInt = llvm::cast<llvm::ConstantInt>(fromConst);
    const llvm::APInt & fromIntVal = fromConstInt->getValue();
    
    // Sign check: if the integer is negative and the destination positive then no cast possible.
    // Note: have to check fromType sign before calling 'isNegative()' here since APInt will always
    // interpret the top bit as the sign bit in a number (llvm does not support unsigned int types).
    if (fromType.isSigned() && fromIntVal.isNegative()) {
        if (!toType.isSigned()) {
            return false;
        }
    }
    
    // Okay, figure out how many bits are required to store the number and how many bits we have
    // to play with in the destination type:
    unsigned bitsRequired = fromIntVal.isNegative() ?
        fromIntVal.getMinSignedBits() :
        fromIntVal.getActiveBits();
    
    unsigned bitsAvailable = toLLVMIntType->getBitWidth();
    
    if (!fromIntVal.isNegative() && toType.isSigned()) {
        // If we are casting from unsigned to signed then we have 1 less bit to play with in the
        // target data type, since that is reserved for the sign bit in 2's complement format:
        bitsAvailable--;
    }
    
    // Implicit cast is allowed if we have enough bits available
    return bitsRequired <= bitsAvailable;
}

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

/* Useful macro */
#define IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(DataTypeName)\
    virtual void visit(const DataTypeName##DataType & dataType) override {\
        WC_UNUSED_PARAM(dataType);\
        mCastAllowed = canImplicitCastIntToIntType(mFromType, mToType, mFromConst);\
    }

/* Useful macro */
#define IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(Int8)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(Int16)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(Int32)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(Int64)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(Int128)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(UInt8)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(UInt16)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(UInt32)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(UInt64)\
    IMPL_CAST_TO_INT_TYPE_IF_POSSIBLE(UInt128)

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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_TYPE_ALWAYS_SUPPORTED(Bool)
    
    virtual void visit(const PtrDataType & dataType) override {
        // Cast to a pointer type is allowed only if the pointer is nullable
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
    
    virtual void visit(const BoolDataType & dataType) override {
        // Note: implicit casts of pointers to bool are technically only allowed if the pointer is
        // nullable, but we allow the cast to be 'attempted' here so that the error will be emitted
        // in just once place, in the casting code.
        WC_UNUSED_PARAM(dataType);
        mCastAllowed = true;
    }
    
    virtual void visit(const PtrDataType & dataType) override {
        // Implicit cast for pointers allowed in the following cases:
        //  (a) regular ptr to nullable regular ptr of same type (e.g ^int -> ?int)
        //  (b) regular ptr to any pointer (e.g ^int -> ^any)
        //  (c) regular ptr to nullable any pointer (e.g ^int -> ?any)
        //  (d) any ptr to nullable any pointer (e.g ^any -> ?any)
        //
        // TODO: Some day we should support implicit casts for structs, classes etc. going from
        // derived types back to base types.
        WC_UNUSED_PARAM(dataType);
        
        const PtrDataType & fromType = static_cast<const PtrDataType&>(mFromType.getDataType());
        const DataType & fromPointedToType = fromType.mPointedToType;
        const PtrDataType & toType = static_cast<const PtrDataType&>(mToType.getDataType());
        const DataType & toPointedToType = toType.mPointedToType;
        
        if (fromPointedToType.isAny()) {
            // Potentially case (d), see if that is so:
            if (toPointedToType.isAny() && !fromType.mIsNullable && toType.mIsNullable) {
                mCastAllowed = true;
            }
        }
        else {
            // Potentially case (a), (b) or (c):
            if (toPointedToType.isAny()) {
                // Case (b) or (c) - allow it:
                mCastAllowed = true;
            }
            else {
                // Potentially case (a), see if that is so:
                if (!fromType.mIsNullable && toType.mIsNullable) {
                    mCastAllowed = true;
                }
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
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
    
    IMPL_CAST_TO_ALL_INT_TYPES_IF_POSSIBLE()
};

//------------------------------------------------------------------------------
// Check cast allowed from: <undefined>
//------------------------------------------------------------------------------
class CheckCastAllowedFromUndefined final : public CheckCastAllowedFromType {
public:
    CheckCastAllowedFromUndefined(const CompiledDataType & fromType,
                                  const CompiledDataType & toType,
                                  llvm::Constant * fromConst)
    :
        CheckCastAllowedFromType(fromType, toType, fromConst)
    {
        WC_EMPTY_FUNC_BODY();
    }
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

void castSingleValueIfRequired(Codegen & cg,
                               Value & value,
                               const CompiledDataType & toTypeCDT)
{
    // If either the value or types are invalid then bail out
    WC_GUARD(value.isValid() && toTypeCDT.isValid());
    
    // If the data type of the value equals the type given then no cast is required.
    // In this case return the value given:
    const CompiledDataType & valueCDT = value.mCompiledType;
    const DataType & valueDT = valueCDT.getDataType();
    const DataType & toTypeDT = toTypeCDT.getDataType();
    WC_GUARD(!valueDT.equals(toTypeDT));
    
    // If the implicit cast is not allowed then do not proceed.
    // In this case also return the value given:
    WC_GUARD(canDoImplicitCast(valueCDT, toTypeCDT, value.mLLVMVal));
    
    // Otherwise do the cast and return the result:
    CodegenCast(cg, value, toTypeCDT).codegen();
    value = cg.mCtx.popValue();
}

void castSingleConstantIfRequired(ConstCodegen & cg,
                                  Constant & constant,
                                  const CompiledDataType & toTypeCDT)
{
    // If either the value or types are invalid then bail out
    WC_GUARD(constant.isValid() && toTypeCDT.isValid());
    
    // If the data type of the value equals the type given then no cast is required.
    // In this case return the value given:
    const CompiledDataType & constantCDT = constant.mCompiledType;
    const DataType & constantDT = constantCDT.getDataType();
    const DataType & toTypeDT = toTypeCDT.getDataType();
    WC_GUARD(!constantDT.equals(toTypeDT));
    
    // If the implicit cast is not allowed then do not proceed.
    // In this case also return the value given:
    WC_GUARD(canDoImplicitCast(constantCDT, toTypeCDT, constant.mLLVMConst));
    
    // Otherwise do the cast and return the result:
    CodegenConstCast(cg, constant, toTypeCDT).codegen();
    constant = cg.mCtx.popConstant();
}

/**
 * Tries to resolve an ambiguous implicit cast for two binary operands.
 * Returns true if the ambiguity was resolved, false otherwise.
 * If the ambiguity is not resolved then an error is emitted, otherwise the function
 * sets to 'false' which of the two casts is NOT to be done.
 */
static bool resolveAmbiguousBinaryOpImplicitCasts(CodegenCtx & cgCtx,
                                                  const DataType & leftType,
                                                  const DataType & rightType,
                                                  bool & castLToROut,
                                                  bool & castRToLOut,
                                                  llvm::Value & leftVal,
                                                  llvm::Value & rightVal)
{
    // See if we are dealing with integer types so we can resolve:
    if (leftType.isInteger() && rightType.isInteger()) {
        // Get the integer types
        const GenericIntDataType & leftIntType = static_cast<const GenericIntDataType&>(leftType);
        const GenericIntDataType & rightIntType = static_cast<const GenericIntDataType&>(rightType);
        
        // See if we can get the left and right values as constant ints
        llvm::ConstantInt * leftConstInt = llvm::dyn_cast<llvm::ConstantInt>(&leftVal);
        llvm::ConstantInt * rightConstInt = llvm::dyn_cast<llvm::ConstantInt>(&rightVal);
        
        // Try to determine whether each side is negative:
        bool leftIsDefinitelyNotNegative = leftConstInt ?
            !leftConstInt->isNegative() :
            false;
        
        bool rightIsDefinitelyNotNegative = rightConstInt ?
            !rightConstInt->isNegative() :
            false;
        
        // Okay, see if either of the integer types is bigger than the other
        if (leftIntType.getIntegerBitCount() > rightIntType.getIntegerBitCount()) {
            // Prefer to cast to the left type because it is bigger.
            //
            // Only allow the cast however in the following circumstances:
            //  (a) The signedness of left and right match
            //  (b) Left is signed and right is unsigned
            //  (c) Left is unsigned and right is signed, but the right value is definitely not negative.
            //
            if (leftIntType.isSigned() == rightIntType.isSigned() ||    // (a)
                !rightIntType.isSigned() ||                             // (b)
                rightIsDefinitelyNotNegative)                           // (c)
            {
                castLToROut = false;
            }
        }
        else if (leftIntType.getIntegerBitCount() < rightIntType.getIntegerBitCount()) {
            // Prefer to cast to the right type because it is bigger.
            //
            // Only allow the cast however in the following circumstances:
            //  (a) The signedness of left and right match
            //  (b) Right is signed and left is unsigned
            //  (c) Right is unsigned and left is signed, but the right value is definitely not negative.
            //
            if (leftIntType.isSigned() == rightIntType.isSigned() ||    // (a)
                !leftIntType.isSigned() ||                              // (b)
                leftIsDefinitelyNotNegative)                            // (c)
            {
                castRToLOut = false;
            }
        }
        else {
            // Bit counts match, prefer the signed type to cast to:
            if (leftIntType.isSigned() != rightIntType.isSigned()) {
                if (leftIntType.isSigned()) {
                    // Prefer to cast to the left because it is signed and the right is unsigned
                    castLToROut = false;
                }
                else {
                    // Prefer to cast to the right because it is signed and the left is unsigned
                    castRToLOut = false;
                }
            }
        }
    }
    
    // See if the ambiguity was resolved:
    if (castLToROut == castRToLOut) {
        cgCtx.error("Ambiguous implicit casts found for a binary expression with a left operand type of '%s' and "
                    "a right operand of type '%s'! Can cast the left type to the right type and visa versa - "
                    "don't know which cast we should pick. Resolve this ambiguity by using the cast() operator!",
                    leftType.name().c_str(),
                    rightType.name().c_str());
        
        return false;
    }
    
    // Resolved the ambiguity!
    return true;
}

BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                            llvm::Value & leftLLVMVal,
                                            const CompiledDataType & leftTypeCDT,
                                            llvm::Value & rightLLVMVal,
                                            const CompiledDataType & rightTypeCDT)
{
    // If either the type is invalid then bail out
    WC_GUARD(leftTypeCDT.isValid() && rightTypeCDT.isValid(), BinaryOpCastDir::kNoCastPossible);
    
    // If the left or right types are the same then bail out:
    const DataType & leftType = leftTypeCDT.getDataType();
    const DataType & rightType = rightTypeCDT.getDataType();
    
    WC_GUARD(!leftType.equals(rightType), BinaryOpCastDir::kNoCastRequired);
    
    // Okay, see which ways we can cast:
    bool canCastLToR = canDoImplicitCast(leftTypeCDT, rightTypeCDT, &leftLLVMVal);
    bool canCastRToL = canDoImplicitCast(rightTypeCDT, leftTypeCDT, &rightLLVMVal);

    // See which ways we can implicitly cast:
    if (canCastLToR) {
        // If we can cast both ways the cast might be ambiguous. We can resolve however for integer types
        // by simply taking the larger of the two types, lets see:
        if (canCastRToL) {
            if (resolveAmbiguousBinaryOpImplicitCasts(cgCtx,
                                                      leftType,
                                                      rightType,
                                                      canCastLToR,
                                                      canCastRToL,
                                                      leftLLVMVal,
                                                      rightLLVMVal))
            {
                if (canCastLToR) {
                    return BinaryOpCastDir::kCastLeftToRight;
                }
                else {
                    return BinaryOpCastDir::kCastRightToLeft;
                }
            }
            else {
                return BinaryOpCastDir::kCastDirIsAmbiguous;
            }
        }
        else {
            return BinaryOpCastDir::kCastLeftToRight;
        }
    }
    else {
        if (canCastRToL) {
            return BinaryOpCastDir::kCastRightToLeft;
        }
    }
    
    return BinaryOpCastDir::kNoCastPossible;
}

BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                            const Value & leftVal,
                                            const Value & rightVal)
{
    // If either the value is invalid then bail out
    WC_GUARD(leftVal.isValid() && rightVal.isValid(), BinaryOpCastDir::kNoCastPossible);
    
    // Determine the cast dir:
    return determineCastDirForBinaryOp(cgCtx,
                                       *leftVal.mLLVMVal,
                                       leftVal.mCompiledType,
                                       *rightVal.mLLVMVal,
                                       rightVal.mCompiledType);
}

BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                            const Constant & leftConst,
                                            const Constant & rightConst)
{
    // If either the value is invalid then bail out
    WC_GUARD(leftConst.isValid() && rightConst.isValid(), BinaryOpCastDir::kNoCastPossible);
    
    // Determine the cast dir:
    return determineCastDirForBinaryOp(cgCtx,
                                       *leftConst.mLLVMConst,
                                       leftConst.mCompiledType,
                                       *rightConst.mLLVMConst,
                                       rightConst.mCompiledType);
}

void castBinaryOpValuesIfRequired(Codegen & cg,
                                  Value & leftVal,
                                  Value & rightVal)
{
    // Determine which way to cast and do it if we can:
    BinaryOpCastDir castDir = determineCastDirForBinaryOp(cg.mCtx, leftVal, rightVal);
    
    if (castDir == BinaryOpCastDir::kCastLeftToRight) {
        CodegenCast(cg, leftVal, rightVal.mCompiledType).codegen();
        leftVal = cg.mCtx.popValue();
    }
    else if (castDir == BinaryOpCastDir::kCastRightToLeft) {
        CodegenCast(cg, rightVal, leftVal.mCompiledType).codegen();
        rightVal = cg.mCtx.popValue();
    }
}

void castBinaryOpValuesIfRequired(ConstCodegen & cg,
                                  Constant & leftConst,
                                  Constant & rightConst)
{
    // Determine which way to cast and do it if we can:
    BinaryOpCastDir castDir = determineCastDirForBinaryOp(cg.mCtx, leftConst, rightConst);
    
    if (castDir == BinaryOpCastDir::kCastLeftToRight) {
        CodegenConstCast(cg, leftConst, rightConst.mCompiledType).codegen();
        leftConst = cg.mCtx.popConstant();
    }
    else if (castDir == BinaryOpCastDir::kCastRightToLeft) {
        CodegenConstCast(cg, rightConst, leftConst.mCompiledType).codegen();
        rightConst = cg.mCtx.popConstant();
    }
}

WC_END_NAMED_NAMESPACE(ImplicitCast)
WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
