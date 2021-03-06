//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "DataType/DataTypeId.hpp"
#include "Macros.hpp"
#include "TokenType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/**
 * Class representing a token. Contains the token type, source information and additional data that
 * varies depending on the token type (if applicable)
 */
struct Token {
    /* The type of token. See enum definition for more details. */
    TokenType type;
    
    /* Line in the source where the token starts, with 0 being the first line. */
    size_t startLine;
    
    /* Column in the source where the token starts, with 0 being the first column */
    size_t startCol;
    
    /* Line in the source where the token ends, with 0 being the first line. */
    size_t endLine;
    
    /* Column in the source where the token ends, with 0 being the first column */
    size_t endCol;
    
    /**
     * Pointer to the location in the source code where the token started.
     * This class does NOT own this memory. And it must remain valid for the lifetime of the token.
     */
    const char * startSrcPtr;
    
    /**
     * Pointer to the location in the source code where the token ended.
     * This class does NOT own this memory. And it must remain valid for the lifetime of the token.
     */
    const char * endSrcPtr;
    
    /* This holds the actual data for the token. Unused/undefined for anything other than a literal or identifier. */
    union {
        /* The raw bytes of the data */
        uint8_t rawBytes[32];
        
        /* Additional data for when the string is interpreted as an int */
        struct IntData {
            /* The string representation of the integer literal minus underscores */
            const char * strMinusUS;
            /* The length of the string representation of the integer literal minus underscores */
            size_t strMinusUSLen;
            /* What base the number is to be interpreted in: 2, 8, 10, 16 */
            size_t base;
            /* What data type the integer literal should be stored in */
            DataTypeId dataTypeId;
        } intData;
        
        /* The data when interpreted as a string */
        struct StrData {
            /* Pointer to the string */
            char * ptr;
            /* Size of the string in code units (byte length) - this is not neccesarily the true length */
            size_t size;
        } strVal;
    } data;
};

WC_END_NAMESPACE
