#include "CompiledDataType.hpp"

#include "Assert.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/InvalidDataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CompiledDataType::CompiledDataType()
:
    mDataType(&PrimitiveDataTypes::getInvalidDataType()),
    mLLVMType(nullptr)
{
    
}

CompiledDataType CompiledDataType::getTransformedToFuncArg() const {
    // Must be valid and require storage for the compiled data type to be transformed for a func arg
    WC_GUARD(isValid() && mDataType->requiresStorage(), *this);
    
    // Alright, get the LLVM type for the arg but as a pointer to that type:
    llvm::Type * newLLVMType = mLLVMType->getPointerTo();
    WC_ASSERT(newLLVMType);
    return CompiledDataType(*mDataType, newLLVMType);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
