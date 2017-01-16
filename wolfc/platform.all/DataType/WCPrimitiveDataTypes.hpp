#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;
enum class DataTypeId : uint8_t;
enum class TokenType : uint8_t;
struct Token;

// TODO: this central store needs to go now that we can modify types with compiles.
// Won't work out once we try to thread out compiling multiple modules at once.

/**
 * Provides access to built-in compiler primitive data types such as int or bool.
 * CAN'T be used to get types such as array or other more advanced types.
 */
namespace PrimitiveDataTypes {
    /** 
     * Get a primitive data type for the given data type id. 
     * Will return the 'unknown' data type if the enum passed in is not valid, or if 
     * the primitive data type cannot be retrieved because it is a user customized type 
     * (e.g array, class).
     */
    DataType & getUsingTypeId(DataTypeId type);
    
    /**
     * Get the primitive data type for a given language keyword, specified as a lexer token type.
     * Returns the 'unknown' data type if the keyword does not correspond to a valid data type.
     */
    DataType & getUsingLangKeyword(TokenType tokenType);
    
    /**
     * Get the default unsigned integer type or the type of the 'uint' alias. Will be a 32-bit
     * unsigned integer for 32-bit targets, and a 64-bit unsigned integer for 64-bit targets.
     */
    DataType & getDefaultUIntType();
    DataTypeId getDefaultUIntTypeId();
    
    /**
     * Get the default signed integer type or the type of the 'int' alias. Will be a 32-bit
     * signed integer for 32-bit targets, and a 64-bit signed integer for 64-bit targets.
     */
    DataType & getDefaultIntType();
    DataTypeId getDefaultIntTypeId();
    
    /**
     * Get the default floating point type or the type of the 'float' alias. Will be 32-bit unless
     * double precision by default is specified.
     */
    DataType & getDefaultFloatType();
    DataTypeId getDefaultFloatTypeId();
}

WC_END_NAMESPACE
