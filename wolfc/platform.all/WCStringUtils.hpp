#pragma once

#include <cstddef>

namespace WCStringUtils {
    /**
     * Convert a null terminated utf8 encoded c string into utf32.
     * Will encode at max 'numUtf8Bytes' but may stop shorter than that if it finds a null terminator.
     */
    char32_t * convertUtf8ToUtf32(const char * utf8Str, size_t numUtf8Bytes);
    
    /**
     * Convert a null terminated utf32 encoded c string into utf8.
     * Will encode at max 'stringLength' but may stop shorter than that if it finds a null terminator.
     */
    char * convertUtf32ToUtf8(const char32_t * utf32Str, size_t stringLength);
}
