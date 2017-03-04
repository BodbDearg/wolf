//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "IntDataTypes.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kInt8DataTypeName = "int8";
static const std::string kInt16DataTypeName = "int16";
static const std::string kInt32DataTypeName = "int32";
static const std::string kInt64DataTypeName = "int64";
static const std::string kInt128DataTypeName = "int128";
static const std::string kUInt8DataTypeName = "uint8";
static const std::string kUInt16DataTypeName = "uint16";
static const std::string kUInt32DataTypeName = "uint32";
static const std::string kUInt64DataTypeName = "uint64";
static const std::string kUInt128DataTypeName = "uint128";

/* A helpful macro to avoid a lot of repetition */
#define IMPL_INT_DATA_TYPE(Name, BitCount)\
\
void Name##DataType::accept(DataTypeVisitor & visitor) const {\
    visitor.visit(*this);\
}\
\
DataTypeId Name##DataType::getTypeId() const {\
    return DataTypeId::k##Name;\
}\
\
const std::string & Name##DataType::name() const {\
    return k##Name##DataTypeName;\
}\
\
bool Name##DataType::equals(const DataType & other) const {\
    return this == &other || dynamic_cast<const Name##DataType*>(&other) != nullptr;\
}\
\
size_t Name##DataType::getIntegerBitCount() const {\
    return BitCount;\
}

/* The integer data type implementations */
IMPL_INT_DATA_TYPE(Int8, 8)
IMPL_INT_DATA_TYPE(Int16, 16)
IMPL_INT_DATA_TYPE(Int32, 32)
IMPL_INT_DATA_TYPE(Int64, 64)
IMPL_INT_DATA_TYPE(Int128, 128)
IMPL_INT_DATA_TYPE(UInt8, 8)
IMPL_INT_DATA_TYPE(UInt16, 16)
IMPL_INT_DATA_TYPE(UInt32, 32)
IMPL_INT_DATA_TYPE(UInt64, 64)
IMPL_INT_DATA_TYPE(UInt128, 128)

/* Done with this */
#undef IMPL_INT_DATA_TYPE

WC_END_NAMESPACE
