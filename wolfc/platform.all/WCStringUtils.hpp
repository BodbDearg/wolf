#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace StringUtils {
    /* Tells if the 1st string starts with the 2nd string */
    bool stringStartsWith(const char * s1, const char * s2);
    
    /**
     * Try to parse a utf8 character at start of the given string. Returns false on failure or true on success.
     * Returns the output unicode character and the number of bytes it is encoded with on success.
     */
    bool parseUtf8Char(const char * utf8Str, char32_t & outputChar, size_t & outputNumEncodingBytes);
    
    /* Give the length of the given null terminated utf32 string. */
    size_t strlen(const char32_t * utf32Str);
    
    /* Compare two utf32 strings lexically. Return 0 if equal. */
    int32_t strcmp(const char32_t * s1, const char32_t * s2);
    
    /**
     * Convert a null terminated utf8 encoded c string into utf32.
     * Will encode at max 'numUtf8Bytes' but may stop shorter than that if it finds a null terminator.
     *
     * N.B Returned pointer must be freed with delete[].
     */
    char32_t * convertUtf8ToUtf32(const char * utf8Str, size_t numUtf8Bytes);
    
    /**
     * Convert a null terminated utf32 encoded c string into utf8.
     * Will encode at max 'stringLength' but may stop shorter than that if it finds a null terminator.
     *
     * N.B Returned pointer must be freed with delete[].
     */
    char * convertUtf32ToUtf8(const char32_t * utf32Str, size_t stringLength);
}

WC_END_NAMESPACE
