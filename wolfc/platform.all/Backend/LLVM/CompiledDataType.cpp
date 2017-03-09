//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CompiledDataType.hpp"

#include "Assert.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/UndefinedDataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CompiledDataType::CompiledDataType()
:
    mDataType(&PrimitiveDataTypes::getUndefinedDataType()),
    mLLVMType(nullptr)
{
    
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
