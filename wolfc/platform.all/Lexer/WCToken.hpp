#pragma once

#include "WCMacros.hpp"
#include "WCTokenType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
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
        uint8_t rawBytes[16];
        
        /* The data when interpreted as a 64-bit int */
        uint64_t intVal;
        
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
