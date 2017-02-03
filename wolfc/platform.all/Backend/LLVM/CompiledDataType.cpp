#include "CompiledDataType.hpp"

#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/UnknownDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CompiledDataType::CompiledDataType()
:
    mDataType(&PrimitiveDataTypes::getUnknownDataType()),
    mLLVMType(nullptr)
{
    
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
