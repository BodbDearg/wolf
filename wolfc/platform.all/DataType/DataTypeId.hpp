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
    /**
     * Unevaluated data type. Means the data type is not yet known, and will have to be determined
     * through further analysis, at compile time.
     */
    kUneval,
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
    /* Array type whose size expression has not yet been evaluated */
    kArrayUnevalSize,
    /* Array type whose size expression has been evaluated but is invalid */
    kArrayBadSize,
    /* Function type */
    kFunc,
    /* Function type whose return and/or argument types have not yet been fully evaluated */
    kFuncUneval,
};

WC_END_NAMESPACE
