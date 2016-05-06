#pragma once

#include <cstddef>

namespace WCStringUtils {
    /**
     * Convert a utf8 encoded string into utf32
     */
    char32_t * convertUtf8ToUtf32(const char * utf8Str, size_t numUtf8Bytes);
    
    /**
     * Convert a utf32 encoded string into utf8
     */
    char * convertUtf32ToUtf8(const char32_t * utf32Str, size_t stringLength);
}
