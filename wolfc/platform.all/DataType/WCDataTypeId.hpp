#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* Enum representing a data type id. Can be used to switch on data types. */
enum class DataTypeId : uint8_t {
    /* Unknown data type. Similar to 'void' but only used for compiler errors. */
    kUnknown,
    /* Void/null data type. No actual data held. */
    kVoid,
    /* Signed 64-bit integer data type */
    kInt64,
    /* Boolean data type */
    kBool,
    /**
     * String constant data type
     * TODO: will probably change once we have a string class and once we support pointers to chars.
     */
    kStr,
    /* Array type */
    kArray,
    /* The number of data type ids */
    kNumTypes
};

WC_END_NAMESPACE
