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

/**
 * Class representing a token. Contains the token type, source information and additional data that
 * varies depending on the token type (if applicable)
 */
class WCToken {
public:
    /**
     * Constructor for NON literal token types. Should NOT be used for literals which have a value.
     * 
     * @param type  The type of token. Must NOT be a literal type.
     * @param srcLocation   Pointer to the location in the source code where the literal occurred.
     *                      This pointer must remain valid for the lifetime of the token.
     *                      The token does NOT assume responsibility for cleaning this up.
     * @param srcLine   Line number in the source where the literal occurred, where '0' is the first line.
     * @param srcCol    Column in the source where the literal occurred, where '0' is the first column.
     */
    inline WCToken(WCTokenType type,
                   const uint32_t * srcPtr,
                   size_t srcLine,
                   size_t srcCol)
    :
        mType(type),
        mSrcPtr(srcPtr),
        mSrcLine(srcLine),
        mSrcCol(srcCol)
    {
        WC_ASSERT(mSrcPtr);
        WC_ASSERT(type != WCTokenType::kUIntLiteral);
    }
    
    /** 
     * Constructor specifically for integer literals. 
     * Similar args to generic non literal constructor, execept for the literal value is 
     * specified instead of the token type.
     */
    inline WCToken(uint64_t uintLiteral,
                   const uint32_t * srcPtr,
                   size_t srcLine,
                   size_t srcCol)
    :
        mType(WCTokenType::kUIntLiteral),
        mSrcPtr(srcPtr),
        mSrcLine(srcLine),
        mSrcCol(srcCol)
    {
        mLiteralData.uintVal = uintLiteral;
    }
    
    /* Give the unsigned integer value. Assumes the token is of this type. */
    inline uint64_t uintVal() const {
        WC_ASSERT(mType == WCTokenType::kUIntLiteral);
        return mLiteralData.uintVal;
    }
    
private:
    /* The type of token. See enum definition for more details. */
    WCTokenType mType;
    
    /**
     * Pointer to the location in the source code where the literal occurred. This class does NOT own this memory.
     * And it must remain valid for the lifetime of the token.
     */
    const uint32_t * mSrcPtr;
    
    /* Line in the source where the token starts, with 0 being the first line. */
    size_t mSrcLine;
    
    /* Column in the source where the token starts, with 0 being the first column */
    size_t mSrcCol;
    
    /** 
     * This holds the actual data for the token. Unused/undefined for anything other than a literal.
     * We don't initialize this in all cases for reasons of efficiency.
     */
    union {
        uint64_t    uintVal;
        uint8_t     bytes[16];
    } mLiteralData;
};
