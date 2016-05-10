#pragma once

#include "WCMacros.hpp"
#include <cstdint>

/* Enum representing the type of token */
enum class WCTokenType {
    kEOF,           // End of file token
    kIntLiteral,    // Unsigned integer literal, like '01203'
    kLParen,        // '('
    kRParen,        // ')'
    kPlus,          // '+'
    kMinus,         // '-'
    kAsterisk,      // '*'
    kSlash,         // '/'
};

/**
 * Class representing a token. Contains the token type, source information and additional data that
 * varies depending on the token type (if applicable)
 */
struct WCToken {
    /* The type of token. See enum definition for more details. */
    WCTokenType mType;
    
    /**
     * Pointer to the location in the source code where the literal occurred. 
     * This class does NOT own this memory. And it must remain valid for the lifetime of the token.
     */
    const char32_t * mSrcPtr;
    
    /* Line in the source where the token starts, with 0 being the first line. */
    size_t mSrcLine;
    
    /* Column in the source where the token starts, with 0 being the first column */
    size_t mSrcCol;
    
    /** 
     * This holds the actual data for the token. Unused/undefined for anything other than a literal.
     * We don't initialize this in all cases for reasons of efficiency.
     */
    union {
        uint64_t    intVal;
        uint8_t     rawBytes[16];
    } mData;
};
