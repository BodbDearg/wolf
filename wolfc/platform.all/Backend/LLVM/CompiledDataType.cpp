#include "CompiledDataType.hpp"

#include "Assert.hpp"
#include "DataType/Types/InvalidDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

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

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
