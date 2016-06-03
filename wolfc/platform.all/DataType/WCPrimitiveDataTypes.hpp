#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class DataType;

/* Provides access to built-in compiler primitive data types */
namespace PrimitiveDataTypes {
    /* Type of primitive data type */
    enum class Type {
        /* Unknown data type. Similar to 'void'. Used sometimes for compiler errors. */
        kUnknown,
        /* Void/null data type. No actual data held. */
        kVoid,
        /* Integer data type */
        kInt,
        /* Boolean data type */
        kBool,
        /* The number of primitive data types */
        kNumTypes
    };
    
    /* Get a primitive data type. Will return the 'unknown' data type if the enum passed in is not valid. */
    DataType & get(Type type);
}

WC_END_NAMESPACE
