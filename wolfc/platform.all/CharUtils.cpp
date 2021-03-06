//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CharUtils.hpp"

WC_BEGIN_NAMESPACE

bool CharUtils::isValidIdentifierStartChar(char32_t c) {
    return isAlpha(c) || isUnderscore(c);
}

bool CharUtils::isValidIdentifierMiddleChar(char32_t c) {
    return isValidIdentifierStartChar(c) || isDecimalDigit(c);
}

bool CharUtils::isAlpha(char32_t c) {
    // See if 7-bit Ascii or unicode
    if (c <= 127) {
        // 7-bit Ascii: 
        if (c >= 'A') {
            if (c <= 'z') {
                // Somewhere between 'A' and 'z', narrow down search:
                if (c >= 'a') {
                    return true;    // Between a-z
                }
                
                if (c <= 'Z') {
                    return true;    // Between A-Z
                }
            }
        }
        
        // Not an alpha char:
        return false;
    }
    
    return !isWhitespace(c);    // Unicode: accept any char that is not whitespace
}

bool CharUtils::isAlphaNumeric(char32_t c) {
    return isDecimalDigit(c) || isAlpha(c);
}

WC_END_NAMESPACE
