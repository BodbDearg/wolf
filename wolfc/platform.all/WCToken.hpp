#pragma once

#include "WCMacros.hpp"
#include <cstdint>

WC_BEGIN_NAMESPACE

/* Enum representing the type of token */
enum class TokenType {
    kEOF,           // End of file token
    kIntLit,        // Unsigned integer literal, like '01203'
    kStrLit,        // String literal, like "Hello World"
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
     * This holds the actual data for the token. Unused/undefined for anything other than a literal.
     * We don't initialize this in all cases for reasons of efficiency.
     */
    union {
        uint8_t rawBytes[16];
        uint64_t intVal;
        
        struct StrData {
            char32_t * ptr;
            size_t length;
        } strVal;
    } data;
};


WC_END_NAMESPACE
