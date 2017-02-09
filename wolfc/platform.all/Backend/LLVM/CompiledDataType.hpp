#pragma once

#include "Macros.hpp"

namespace llvm {
    class Type;
}

WC_BEGIN_NAMESPACE

class DataType;

WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * Class representing a compiled and evaluated data type. A compiled data type is a data type that has had any expressions
 * determining the exact type compiled (such as an array size expression) as well as the actual llvm type determined.
 */
class CompiledDataType {
public:
    CompiledDataType();
    
    inline CompiledDataType(const DataType & dataType, llvm::Type * llvmType) :
        mDataType(&dataType),
        mLLVMType(llvmType)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    inline CompiledDataType(const CompiledDataType & other) = default;
    inline CompiledDataType & operator = (const CompiledDataType & other) = default;
    
    inline const DataType & getDataType() const {
        return *mDataType;
    }
    
    inline llvm::Type * getLLVMType() const {
        return mLLVMType;
    }
    
    inline bool isValid() const {
        return mDataType != nullptr && mLLVMType != nullptr;
    }
    
    /**
     * Performs transformations to the LLVM type for this data type required for function args.
     * Returns the transformed version of the compiled data type.
     *
     * For data types that require stack storage (Arrays, Structs etc.) we pass them into functions by
     * pointer (secretly!) even when they are passed by value. This follows the behavior of the C/Stdcall 
     * etc. calling conventions.
     */
    CompiledDataType getTransformedToFuncArg() const;
    
private:
    /* The data type object for this type */
    const DataType * mDataType;
    
    /* The llvm type representing the data type */
    llvm::Type * mLLVMType;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
