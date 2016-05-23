#pragma once

#include "WCMacros.hpp"
#include <cstddef>

WC_BEGIN_NAMESPACE

/* Utilities for querying character classes. */
namespace CharUtils {
    /* Tells if a unicode character is valid to be used in an identifier as the first character. */
    bool isValidIdentifierStartChar(char32_t c);
    
    /* Tells if a unicode character is valid to be used in an identifier for characters other than the start. */
    bool isValidIdentifierMiddleChar(char32_t c);
    
    /* Tell if a unicode character is an alpha (regular letter) character. */
    bool isAlpha(char32_t c);
    
    /* Tells if a unicode character is an underscore */
    inline constexpr bool isUnderscore(char32_t c) {
        return c == '_';
    }
    
    /* Tells if a unicode character is 0-9 */
    inline constexpr bool isDigit(char32_t c) {
        return c >= '0' && c <= '9';
    }
    
    /* Tells if a unicode character is a line separator */
    inline constexpr bool isLineSeparator(char32_t c) {
        // See: https://en.wikipedia.org/wiki/Newline
        switch (c) {
            case 0x000A:        // Line Feed (LF)
            case 0x000B:        // Vertical tab (VT)
            case 0x000C:        // Form Feed (FF)
            case 0x000D:        // Carriage Return (CR)
            case 0x0085:        // Next Line (NEL)
            case 0x2028:        // Line Separator (LS)
            case 0x2029:        // Paragraph Separator (PS)
                return true;
                
            default:
                break;
        }
        
        return false;
    }
    
    /* Tells if a unicode character is a whitespace character */
    inline constexpr bool isWhitespace(char32_t c) {
        // See: https://en.wikipedia.org/wiki/Whitespace_character
        switch (c) {
            case 0x0009:        // Horizontal tab (HT)
            case 0x000A:        // Line Feed (LF)
            case 0x000B:        // Vertical tab (VT)
            case 0x000C:        // Form Feed (FF)
            case 0x000D:        // Carriage Return (CR)
            case 0x0020:        // Space
            case 0x0085:        // Next Line (NEL)
            case 0x00A0:        // Non breaking space
            case 0x1680:        // Ogham space mark
            case 0x180E:        // Mongolian vowel separator
            case 0x2000:        // En quad
            case 0x2001:        // Em quad
            case 0x2002:        // En space
            case 0x2003:        // Em space
            case 0x2004:        // Three-per-em space
            case 0x2005:        // Four-per-em space
            case 0x2006:        // Six-per-em space
            case 0x2007:        // Figure space
            case 0x2008:        // Punctuation space
            case 0x2009:        // Thin space
            case 0x200A:        // Hair space
            case 0x200B:        // Zero width space
            case 0x200C:        // Zero width non-joiner
            case 0x200D:        // Zero width joiner
            case 0x2028:        // Line separator
            case 0x2029:        // Paragraph separator
            case 0x202F:        // Narrow no-break space
            case 0x205F:        // Medium mathematical space
            case 0x2060:        // Word joiner
            case 0x3000:        // Ideographic space
            case 0xFEFF:        // Zero width non-breaking
                return true;
                
            default:
                break;
        }
        
        return false;
    }
}

WC_END_NAMESPACE
