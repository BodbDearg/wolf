#pragma once

#include "GenericSignedIntDataType.hpp"
#include "GenericUnsignedIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Helpful macro to avoid repetition */
#define DECLARE_INT_DATA_TYPE(Name, BaseDataType)\
    class Name##DataType final : public BaseDataType {\
    public:\
        virtual void accept(DataTypeVisitor & visitor) const override;\
        virtual DataTypeId getTypeId() const override;\
        virtual const std::string & name() const override;\
        virtual bool equals(const DataType & other) const override;\
    };

/* The integer data types */
DECLARE_INT_DATA_TYPE(Int8, GenericSignedIntDataType)
DECLARE_INT_DATA_TYPE(Int16, GenericSignedIntDataType)
DECLARE_INT_DATA_TYPE(Int32, GenericSignedIntDataType)
DECLARE_INT_DATA_TYPE(Int64, GenericSignedIntDataType)
DECLARE_INT_DATA_TYPE(Int128, GenericSignedIntDataType)
DECLARE_INT_DATA_TYPE(UInt8, GenericUnsignedIntDataType)
DECLARE_INT_DATA_TYPE(UInt16, GenericUnsignedIntDataType)
DECLARE_INT_DATA_TYPE(UInt32, GenericUnsignedIntDataType)
DECLARE_INT_DATA_TYPE(UInt64, GenericUnsignedIntDataType)
DECLARE_INT_DATA_TYPE(UInt128, GenericUnsignedIntDataType)

/* Done with this */
#undef DECLARE_INT_DATA_TYPE

WC_END_NAMESPACE
