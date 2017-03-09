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

class AnyDataType;
class BoolDataType;
class DataType;
class NullptrDataType;
class PtrDataType;
class UndefinedDataType;
class VoidDataType;
enum class DataTypeId : uint8_t;
enum class TokenType : uint8_t;
struct Token;

/**
 * Provides access to built-in compiler primitive data types such as int or bool.
 * CAN'T be used to get types such as array or other more advanced types.
 */
namespace PrimitiveDataTypes {
    /** 
     * Get a primitive data type for the given data type id. 
     * Will return the 'undefined' data type if the enum passed in is not valid, or if
     * the primitive data type cannot be retrieved because it is a user customized type 
     * (e.g array, class).
     */
    const DataType & getUsingTypeId(DataTypeId type);
    
    /**
     * Get the primitive data type id for a given language keyword, specified as a lexer token type.
     * Returns the 'undefined' data type id if the keyword does not correspond to a primitive data ty[e.
     */
    DataTypeId getTypeIdForLangKeyword(TokenType tokenType);
    
    /**
     * Get the primitive data type for a given language keyword, specified as a lexer token type.
     * Returns the 'undefined' data type if the keyword does not correspond to a primitive data ty[e.
     */
    const DataType & getUsingLangKeyword(TokenType tokenType);
    
    /* Tells if the given language token is a primitive type. */
    bool isLangKeywordPrimitiveType(TokenType tokenType);
    
    /* Get the '<undefined>' data type */
    const UndefinedDataType & getUndefinedDataType();

    /* Get the 'void' data type */
    const VoidDataType & getVoidDataType();
    
    /* Get the 'any' data type */
    const AnyDataType & getAnyDataType();
    
    /* Get the 'nullptr' data type */
    const NullptrDataType & getNullptrDataType();
    
    /* Get the 'bool' data type */
    const BoolDataType & getBoolDataType();
    
    /**
     * Get the default unsigned integer type or the type of the 'uint' alias. Will be a 32-bit
     * unsigned integer for 32-bit targets, and a 64-bit unsigned integer for 64-bit targets.
     */
    const DataType & getDefaultUIntType();
    DataTypeId getDefaultUIntTypeId();
    
    /**
     * Get the default signed integer type or the type of the 'int' alias. Will be a 32-bit
     * signed integer for 32-bit targets, and a 64-bit signed integer for 64-bit targets.
     */
    const DataType & getDefaultIntType();
    DataTypeId getDefaultIntTypeId();
    
    /**
     * Get the default floating point type or the type of the 'float' alias. Will be 32-bit unless
     * double precision by default is specified.
     */
    const DataType & getDefaultFloatType();
    DataTypeId getDefaultFloatTypeId();
}

WC_END_NAMESPACE
