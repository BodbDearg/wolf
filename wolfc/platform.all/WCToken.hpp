#pragma once

#include "WCMacros.hpp"
#include <cstdint>

/* Enum representing the type of token */
enum class WCTokenType {
    kUIntLiteral,   // Unsigned integer literal, like '01203'
    kLParen,        // '('
    kRParen,        // ')'
    kPlus,          // '+'
    kMinus,         // '-'
    kAsterisk,      // '*'
    kSlash,         // '/'
};

/* Class representing a token. Contains the token type and additional data (if applicable) */
class WCToken {
public:
    /* Constructor for NON literal token types. Should NOT be used for literals which have a value. */
    inline WCToken(WCTokenType type) : mType(type) {
        WC_ASSERT(type != WCTokenType::kUIntLiteral);
    }
    
    /* Constructor for integer literals */
    inline WCToken(uint64_t uintLiteral) : mType(WCTokenType::kUIntLiteral) {
        mLiteralData.uintVal = uintLiteral;
    }
    
    /* Give the unsigned integer value */
    inline uint64_t uintVal() const {
        WC_ASSERT(mType == WCTokenType::kUIntLiteral);
        return mLiteralData.uintVal;
    }
    
private:
    /* The type of token */
    WCTokenType mType;
    
    /* This holds the actual data for the token. Unused/undefined for anything other than a literal. */
    union {
        uint64_t    uintVal;
        uint8_t     bytes[16];
    } mLiteralData;
};
