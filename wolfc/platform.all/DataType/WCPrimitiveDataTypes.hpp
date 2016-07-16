#pragma once

#include "WCMacros.hpp"
#include "WCTokenType.hpp"

WC_BEGIN_NAMESPACE

class DataType;
struct Token;

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
        /**
         * String constant data type
         * TODO: will probably change once we have a string class and once we support pointers to chars.
         */
        kStr,
        /* The number of primitive data types */
        kNumTypes
    };
    
    /* Get a primitive data type. Will return the 'unknown' data type if the enum passed in is not valid. */
    DataType & get(Type type);
    
    /**
     * Get the primitive data type for a given language keyword, specified as a lexer token type.
     * Returns the 'unknown' data type if the keyword does not correspond to a valid data type.
     */
    DataType & dataTypeForLangKeyword(TokenType tokenType);
}

WC_END_NAMESPACE
