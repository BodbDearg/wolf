//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

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
    /* Signed 8-bit integer data type */
    kInt8,
    /* Signed 16-bit integer data type */
    kInt16,
    /* Signed 32-bit integer data type */
    kInt32,
    /* Signed 64-bit integer data type */
    kInt64,
    /* Signed 128-bit integer data type */
    kInt128,
    /* Unsigned 8-bit integer data type */
    kUInt8,
    /* Unsigned 16-bit integer data type */
    kUInt16,
    /* Unsigned 32-bit integer data type */
    kUInt32,
    /* Unsigned 64-bit integer data type */
    kUInt64,
    /* Unsigned 128-bit integer data type */
    kUInt128,
    /**
     * String constant data type
     * TODO: will probably change once we have a string class and once we support pointers to chars.
     */
    kStr,
    /* Pointer type */
    kPtr,
    /* Array type */
    kArray,
    /* Function type */
    kFunc
};

WC_END_NAMESPACE
