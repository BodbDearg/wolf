#include "WCStringUtils.hpp"
#include "WCMacros.hpp"

char32_t * WCStringUtils::convertUtf8ToUtf32(const char * utf8Str, size_t numUtf8Bytes) {
    WC_ASSERT(utf8Str);
    
    // Don't know how long the actual string will be so just use the byte length. This may not be the most
    // space efficient solution but deliberately sacraficing space here for speed... Don't want to have
    // multiple allocs and possible array copying.
    char32_t * utf32Str = new char32_t[numUtf8Bytes + 1];
    
    while (numUtf8Bytes > 0) {
        char utf8Byte1 = *utf8Str;
        
        if (utf8Byte1 & 0x80) {
            // Possible multi byte sequence detected: Bit pattern 1???????
            if (utf8Byte1 & 0x40) {
                // Two or more bytes encoding:
                if (utf8Byte1 & 0x20) {
                    // Three or more bytes encoding:
                    if (utf8Byte1 & 0x10) {
                        // Four or more bytes encoding:
                        if (utf8Byte1 & 0x08) {
                            // Five or more bytes encoding:
                            if (utf8Byte1 & 0x04) {
                                // Six byte encoding:
                                if (numUtf8Bytes >= 6) {
                                    char32_t utf32Char = (utf8Byte1 & 0x01) << 30;
                                    utf32Char |= (utf8Str[1] & 0x3F) << 24;
                                    utf32Char |= (utf8Str[2] & 0x3F) << 18;
                                    utf32Char |= (utf8Str[3] & 0x3F) << 12;
                                    utf32Char |= (utf8Str[4] & 0x3F) << 6;
                                    utf32Char |= (utf8Str[5] & 0x3F) << 6;
                                    
                                    *utf32Str = utf32Char;
                                    ++utf32Str;
                                    utf8Str += 6;
                                    numUtf8Bytes -= 6;
                                }
                                else {
                                    break;  // Unexpected end of string reached due to NULL terminator.
                                }
                            }
                            else {
                                // Five byte encoding:
                                if (numUtf8Bytes >= 5) {
                                    char32_t utf32Char = (utf8Byte1 & 0x03) << 24;
                                    utf32Char |= (utf8Str[1] & 0x3F) << 18;
                                    utf32Char |= (utf8Str[2] & 0x3F) << 12;
                                    utf32Char |= (utf8Str[3] & 0x3F) << 6;
                                    utf32Char |= (utf8Str[4] & 0x3F);
                                    
                                    *utf32Str = utf32Char;
                                    ++utf32Str;
                                    utf8Str += 5;
                                    numUtf8Bytes -= 5;
                                }
                                else {
                                    break;  // Unexpected end of string reached due to NULL terminator.
                                }
                            }
                        }
                        else {
                            // Four byte encoding:
                            if (numUtf8Bytes >= 4) {
                                char32_t utf32Char = (utf8Byte1 & 0x07) << 18;
                                utf32Char |= (utf8Str[1] & 0x3F) << 12;
                                utf32Char |= (utf8Str[2] & 0x3F) << 6;
                                utf32Char |= (utf8Str[3] & 0x3F);
                                
                                *utf32Str = utf32Char;
                                ++utf32Str;
                                utf8Str += 4;
                                numUtf8Bytes -= 4;
                            }
                            else {
                                break;  // Unexpected end of string reached due to NULL terminator.
                            }
                        }
                    }
                    else {
                        // Three byte encoding:
                        if (numUtf8Bytes >= 3) {
                            char32_t utf32Char = (utf8Byte1 & 0x0F) << 12;
                            utf32Char |= (utf8Str[1] & 0x3F) << 6;
                            utf32Char |= (utf8Str[2] & 0x3F);
                            
                            *utf32Str = utf32Char;
                            ++utf32Str;
                            utf8Str += 3;
                            numUtf8Bytes -= 3;
                        }
                        else {
                            break;  // Unexpected end of string reached due to NULL terminator.
                        }
                    }
                }
                else {
                    // Two byte encoding: do a safety check on subsequent bytes however to make sure they're not null
                    if (numUtf8Bytes >= 2) {
                        char32_t utf32Char = (utf8Byte1 & 0x1F) << 6;
                        utf32Char |= (utf8Str[1] & 0x3F);
                        
                        *utf32Str = utf32Char;
                        ++utf32Str;
                        utf8Str += 2;
                        numUtf8Bytes -= 2;
                    }
                    else {
                        break;  // Unexpected end of string reached due to NULL terminator.
                    }
                }
            }
            else {
                // Bad leading byte for UTF8 - must have bit pattern 11?????? or the first two high order bits as '1'.
                // Skip this byte to try to recover:
                ++utf8Str;
                --numUtf8Bytes;
            }
        }
        else {
            // Single byte character: Bit pattern 0???????
            // Check however we haven't reached NULL. If that is the case then stop.
            if (utf8Byte1 != 0) {
                *utf32Str = utf8Byte1;
                
                ++utf32Str;
                ++utf8Str;
                --numUtf8Bytes;
            }
            else {
                // NULL terminator reached, stop conversion.
                break;
            }
        }
    }
    
    // NULL terminate the utf32 string and return
    *utf32Str = 0;
    return utf32Str;
}

char * WCStringUtils::convertUtf32ToUtf8(const char32_t * utf32Str, size_t stringLength) {
    WC_ASSERT(utf32Str);
    
    // Don't know yet what size in bytes actual string will be so just use the length * 6. This may not
    // be the most space efficient solution but deliberately sacraficing space here for speed... Don't want
    // to have multiple allocs and possible array copying.
    char * utf8Str = new char[stringLength + 1];
    
    while (stringLength > 0) {
        uint32_t utf32Char = utf32Str[0];
        
        if (utf32Char < 0x80) {
            // 1 byte encoded character
            // Check however we haven't reached NULL. If that is the case then stop.
            if (utf32Char != 0) {
                utf8Str[0] = static_cast<char>(utf32Char);
                ++utf8Str;
            }
            else {
                // NULL terminator reached, stop conversion.
                break;
            }
        }
        else if (utf32Char < 0x800) {
            // 2 byte encoded character
            utf8Str[0] = 0xC0 | (utf32Char >> 6);
            utf8Str[1] = 0x80 | (utf32Char & 0x3F);
            utf8Str += 2;
        }
        else if (utf32Char < 0x10000) {
            // 3 byte encoded character
            utf8Str[0] = 0xE0 | (utf32Char >> 12);
            utf8Str[1] = 0x80 | ((utf32Char >> 6) & 0x3F);
            utf8Str[2] = 0x80 | (utf32Char & 0x3F);
            utf8Str += 3;
        }
        else if (utf32Char < 0x200000) {
            // 4 byte encoded character
            utf8Str[0] = 0xF0 | (utf32Char >> 18);
            utf8Str[1] = 0x80 | ((utf32Char >> 12) & 0x3F);
            utf8Str[2] = 0x80 | ((utf32Char >> 6) & 0x3F);
            utf8Str[3] = 0x80 | (utf32Char & 0x3F);
            utf8Str += 4;
        }
        else if (utf32Char < 0x4000000) {
            // 5 byte encoded character
            utf8Str[0] = 0xF8 | (utf32Char >> 24);
            utf8Str[1] = 0x80 | ((utf32Char >> 18) & 0x3F);
            utf8Str[2] = 0x80 | ((utf32Char >> 12) & 0x3F);
            utf8Str[3] = 0x80 | ((utf32Char >> 6) & 0x3F);
            utf8Str[4] = 0x80 | (utf32Char & 0x3F);
            utf8Str += 5;
        }
        else if (utf32Char < 0x80000000) {
            // 6 byte encoded character
            utf8Str[0] = 0xFC | (utf32Char >> 30);
            utf8Str[1] = 0x80 | ((utf32Char >> 24) & 0x3F);
            utf8Str[2] = 0x80 | ((utf32Char >> 18) & 0x3F);
            utf8Str[3] = 0x80 | ((utf32Char >> 12) & 0x3F);
            utf8Str[4] = 0x80 | ((utf32Char >> 6) & 0x3F);
            utf8Str[5] = 0x80 | (utf32Char & 0x3F);
            utf8Str += 6;
        }
        
        ++utf32Str;
        --stringLength;
    }
    
    // Null terminate the utf8 string and return
    *utf8Str = 0;
    return utf8Str;
}
