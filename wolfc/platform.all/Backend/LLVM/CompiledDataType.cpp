#include "CompiledDataType.hpp"

#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/InvalidDataType.hpp"

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
