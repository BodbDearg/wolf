#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* Enum representing a data type id. Can be used to switch on data types. */
enum class DataTypeId : uint8_t {
    /* Invalid data type. Used for compiler errors, when we can't tell the type of something. */
    kInvalid,
    /* Void/null data type. No actual data held. */
    kVoid,
    /* Boolean data type */
    kBool,
    /* Signed 64-bit integer data type */
    kInt64,
    /**
     * String constant data type
     * TODO: will probably change once we have a string class and once we support pointers to chars.
     */
    kStr,
    /* Array type */
    kArray,
    /* Function type */
    kFunc
};

WC_END_NAMESPACE
