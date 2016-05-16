#pragma once

#include "WCMacros.hpp"
#include "WCTokenType.hpp"
#include <cstdint>

WC_BEGIN_NAMESPACE

/**
 * Class representing a token. Contains the token type, source information and additional data that
 * varies depending on the token type (if applicable)
 */
struct Token {
    /* The type of token. See enum definition for more details. */
    TokenType type;
    
    /**
     * Pointer to the location in the source code where the literal occurred. 
     * This class does NOT own this memory. And it must remain valid for the lifetime of the token.
     */
    const char32_t * srcPtr;
    
    /* Line in the source where the token starts, with 0 being the first line. */
    size_t srcLine;
    
    /* Column in the source where the token starts, with 0 being the first column */
    size_t srcCol;
    
    /** 
     * This holds the actual data for the token. Unused/undefined for anything other than a literal or identifier.
     * We don't initialize this in all cases for reasons of efficiency.
     */
    union {
        /* The raw bytes of the data */
        uint8_t rawBytes[16];
        
        /* The data when interpreted as a 64-bit int */
        uint64_t intVal;
        
        /* The data when interpreted as a string */
        struct StrData {
            /* Pointer to the string */
            char32_t * ptr;
            /* Length of the string */
            size_t length;
        } strVal;
    } data;
};


WC_END_NAMESPACE
