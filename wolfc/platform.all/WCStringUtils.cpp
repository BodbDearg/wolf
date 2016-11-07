#include "WCStringUtils.hpp"

#include "WCAssert.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool StringUtils::stringStartsWith(const char * s1, const char * s2) {
    WC_ASSERT(s1);
    WC_ASSERT(s2);
    
    while (true) {
        // Grab the val of both chars at this position
        char c1 = s1[0];
        char c2 = s2[0];
        
        // See if not equal
        if (c1 != c2) {
            // If s2 has ended before s1 and they were equal so far then s2 starts with s1!
            if (c2 == 0) {
                return true;
            }
            
            // Otherwise we found an inequality
            return false;
        }
        
        // If we reached the end of s2 and s1 then break out and return true
        if (c2 == 0) {
            break;
        }
        
        ++s1;
        ++s2;
    }
    
    return true;    // s1 and s2 are same length and equal
}

bool StringUtils::parseUtf8Char(const char * utf8Str, char32_t & outputChar, size_t & outputNumEncodingBytes) {
    WC_ASSERT(utf8Str);
    uint8_t utf8Byte1 = static_cast<uint8_t>(*utf8Str);
    
    if (utf8Byte1 & 0x80) {
        // Possible multi byte sequence detected: Bit pattern 1???????
        if (utf8Byte1 & 0x40) {
            // Two or more bytes encoding: get the 2nd byte
            uint8_t utf8Byte2 = static_cast<uint8_t>(utf8Str[1]);
            
            if (utf8Byte2 == 0) {
                // Error! Unexpected NULL character reached in utf8 sequence.
                outputChar = 0;
                outputNumEncodingBytes = 0;
                return false;
            }
            
            if (utf8Byte1 & 0x20) {
                // Three or more bytes encoding: get the 3rd byte
                uint8_t utf8Byte3 = static_cast<uint8_t>(utf8Str[2]);
                
                if (utf8Byte3 == 0) {
                    // Error! Unexpected NULL character reached in utf8 sequence.
                    outputChar = 0;
                    outputNumEncodingBytes = 0;
                    return false;
                }
                
                if (utf8Byte1 & 0x10) {
                    // Four or more bytes encoding: get the 4th byte
                    uint8_t utf8Byte4 = static_cast<uint8_t>(utf8Str[3]);
                    
                    if (utf8Byte4 == 0) {
                        // Error! Unexpected NULL character reached in utf8 sequence.
                        outputChar = 0;
                        outputNumEncodingBytes = 0;
                        return false;
                    }
                    
                    if (utf8Byte1 & 0x08) {
                        // Five or more bytes encoding: get the 5th byte
                        uint8_t utf8Byte5 = static_cast<uint8_t>(utf8Str[4]);
                        
                        if (utf8Byte5 == 0) {
                            // Error! Unexpected NULL character reached in utf8 sequence.
                            outputChar = 0;
                            outputNumEncodingBytes = 0;
                            return false;
                        }
                        
                        if (utf8Byte1 & 0x04) {
                            // Six byte encoding: get the 6th byte
                            uint8_t utf8Byte6 = static_cast<uint8_t>(utf8Str[5]);
                            
                            if (utf8Byte6 == 0) {
                                // Error! Unexpected NULL character reached in utf8 sequence.
                                outputChar = 0;
                                outputNumEncodingBytes = 0;
                                return false;
                            }
                            
                            uint32_t utf32Char = (utf8Byte1 & 0x01u) << 30;
                            utf32Char |= (utf8Byte2 & 0x3Fu) << 24;
                            utf32Char |= (utf8Byte3 & 0x3Fu) << 18;
                            utf32Char |= (utf8Byte4 & 0x3Fu) << 12;
                            utf32Char |= (utf8Byte5 & 0x3Fu) << 6;
                            utf32Char |= (utf8Byte6 & 0x3Fu) << 6;
                            outputChar = static_cast<char32_t>(utf32Char);
                            outputNumEncodingBytes = 6;
                        }
                        else {
                            // Five byte encoding:
                            uint32_t utf32Char = (utf8Byte1 & 0x03u) << 24;
                            utf32Char |= (utf8Byte2 & 0x3Fu) << 18;
                            utf32Char |= (utf8Byte3 & 0x3Fu) << 12;
                            utf32Char |= (utf8Byte4 & 0x3Fu) << 6;
                            utf32Char |= (utf8Byte5 & 0x3Fu);
                            outputChar = static_cast<char32_t>(utf32Char);
                            outputNumEncodingBytes = 5;
                        }
                    }
                    else {
                        // Four byte encoding:
                        uint32_t utf32Char = (utf8Byte1 & 0x07u) << 18;
                        utf32Char |= (utf8Byte2 & 0x3Fu) << 12;
                        utf32Char |= (utf8Byte3 & 0x3Fu) << 6;
                        utf32Char |= (utf8Byte4 & 0x3Fu);
                        outputChar = static_cast<char32_t>(utf32Char);
                        outputNumEncodingBytes = 4;
                    }
                }
                else {
                    // Three byte encoding:
                    uint32_t utf32Char = (utf8Byte1 & 0x0Fu) << 12;
                    utf32Char |= (utf8Byte2 & 0x3Fu) << 6;
                    utf32Char |= (utf8Byte3 & 0x3Fu);
                    outputChar = static_cast<char32_t>(utf32Char);
                    outputNumEncodingBytes = 3;
                }
            }
            else {
                // Two byte encoding:
                uint32_t utf32Char = (utf8Byte1 & 0x1Fu) << 6;
                utf32Char |= (utf8Byte2 & 0x3Fu);
                outputChar = static_cast<char32_t>(utf32Char);
                outputNumEncodingBytes = 2;
            }
        }
        else {
            // Bad leading byte for UTF8 - must have bit pattern 11?????? or the first two high order bits as '1'.
            outputChar = 0;
            outputNumEncodingBytes = 0;
            return false;
        }
    }
    else {
        // Single byte character: Bit pattern 0???????
        outputChar = static_cast<uint8_t>(utf8Byte1);
        outputNumEncodingBytes = 1;
    }
    
    // IF we get to here then we are successful
    return true;
}

size_t StringUtils::strlen(const char32_t * utf32Str) {
    WC_ASSERT(utf32Str);
    size_t length = 0;
    
    while (*utf32Str) {
        ++utf32Str;
        ++length;
    }
    
    return length;
}

int32_t StringUtils::strcmp(const char32_t * s1, const char32_t * s2) {
    WC_ASSERT(s1);
    WC_ASSERT(s2);
    
    while (true) {
        char32_t c1 = s1[0];
        char32_t c2 = s2[0];
        int32_t diff = static_cast<int32_t>(c1) - static_cast<int32_t>(c2);
        
        if (diff != 0) {
            return diff;
        }
        
        if (c1 == 0) {
            if (c2 == 0) {
                break;  // End of both strings
            }
        }
        
        ++s1;
        ++s2;
    }
    
    return 0;
}

char32_t * StringUtils::convertUtf8ToUtf32(const char * utf8Str, size_t numUtf8Bytes) {
    WC_ASSERT(utf8Str);
    
    // Don't know how long the actual string will be so just use the byte length. This may not be the most
    // space efficient solution but deliberately sacraficing space here for speed... Don't want to have
    // multiple allocs and possible array copying.
    char32_t * utf32StrBuffer = new char32_t[numUtf8Bytes + 1];
    char32_t * utf32StrPtr = utf32StrBuffer;
    
    while (numUtf8Bytes > 0) {
        uint8_t utf8Byte1 = static_cast<uint8_t>(*utf8Str);
        
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
                                    uint32_t utf32Char = (utf8Byte1 & 0x01u) << 30;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[1]) & 0x3Fu) << 24;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[2]) & 0x3Fu) << 18;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[3]) & 0x3Fu) << 12;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[4]) & 0x3Fu) << 6;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[5]) & 0x3Fu) << 6;
                                    
                                    *utf32StrPtr = static_cast<char32_t>(utf32Char);
                                    ++utf32StrPtr;
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
                                    uint32_t utf32Char = (utf8Byte1 & 0x03u) << 24;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[1]) & 0x3Fu) << 18;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[2]) & 0x3Fu) << 12;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[3]) & 0x3Fu) << 6;
                                    utf32Char |= (static_cast<uint8_t>(utf8Str[4]) & 0x3Fu);
                                    
                                    *utf32StrPtr = static_cast<char32_t>(utf32Char);
                                    ++utf32StrPtr;
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
                                uint32_t utf32Char = (utf8Byte1 & 0x07u) << 18;
                                utf32Char |= (static_cast<uint8_t>(utf8Str[1]) & 0x3Fu) << 12;
                                utf32Char |= (static_cast<uint8_t>(utf8Str[2]) & 0x3Fu) << 6;
                                utf32Char |= (static_cast<uint8_t>(utf8Str[3]) & 0x3Fu);
                                
                                *utf32StrPtr = static_cast<char32_t>(utf32Char);
                                ++utf32StrPtr;
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
                            uint32_t utf32Char = (utf8Byte1 & 0x0Fu) << 12;
                            utf32Char |= (static_cast<uint8_t>(utf8Str[1]) & 0x3Fu) << 6;
                            utf32Char |= (static_cast<uint8_t>(utf8Str[2]) & 0x3Fu);
                            
                            *utf32StrPtr = static_cast<char32_t>(utf32Char);
                            ++utf32StrPtr;
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
                        uint32_t utf32Char = (utf8Byte1 & 0x1Fu) << 6;
                        utf32Char |= (static_cast<uint8_t>(utf8Str[1]) & 0x3Fu);
                        
                        *utf32StrPtr = static_cast<char32_t>(utf32Char);
                        ++utf32StrPtr;
                        utf8Str += 2;
                        numUtf8Bytes -= 2;
                    }
                    else {
                        break;  // Unexpected end of string reached due to NULL terminator.
                    }
                }
            }
            else {
                // TODO: should we just error out rather than trying to recover?!
                
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
                *utf32StrPtr = utf8Byte1;
                
                ++utf32StrPtr;
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
    *utf32StrPtr = 0;
    return utf32StrBuffer;
}

char * StringUtils::convertUtf32ToUtf8(const char32_t * utf32Str, size_t stringLength) {
    WC_ASSERT(utf32Str);
    
    // Don't know yet what size in bytes actual string will be so just use the length * 6. This may not
    // be the most space efficient solution but deliberately sacraficing space here for speed... Don't want
    // to have multiple allocs and possible array copying.
    char * utf8StrBuffer = new char[(stringLength * 6) + 1];
    char * utf8StrPtr = utf8StrBuffer;
    
    while (stringLength > 0) {
        uint32_t utf32Char = utf32Str[0];
        
        if (utf32Char < 0x80) {
            // 1 byte encoded character
            // Check however we haven't reached NULL. If that is the case then stop.
            if (utf32Char != 0) {
                utf8StrPtr[0] = static_cast<char>(utf32Char);
                ++utf8StrPtr;
            }
            else {
                // NULL terminator reached, stop conversion.
                break;
            }
        }
        else if (utf32Char < 0x800) {
            // 2 byte encoded character
            utf8StrPtr[0] = static_cast<char>(0xC0 | (utf32Char >> 6));
            utf8StrPtr[1] = static_cast<char>(0x80 | (utf32Char & 0x3F));
            utf8StrPtr += 2;
        }
        else if (utf32Char < 0x10000) {
            // 3 byte encoded character
            utf8StrPtr[0] = static_cast<char>(0xE0 | (utf32Char >> 12));
            utf8StrPtr[1] = static_cast<char>(0x80 | ((utf32Char >> 6) & 0x3F));
            utf8StrPtr[2] = static_cast<char>(0x80 | (utf32Char & 0x3F));
            utf8StrPtr += 3;
        }
        else if (utf32Char < 0x200000) {
            // 4 byte encoded character
            utf8StrPtr[0] = static_cast<char>(0xF0 | (utf32Char >> 18));
            utf8StrPtr[1] = static_cast<char>(0x80 | ((utf32Char >> 12) & 0x3F));
            utf8StrPtr[2] = static_cast<char>(0x80 | ((utf32Char >> 6) & 0x3F));
            utf8StrPtr[3] = static_cast<char>(0x80 | (utf32Char & 0x3F));
            utf8StrPtr += 4;
        }
        else if (utf32Char < 0x4000000) {
            // 5 byte encoded character
            utf8StrPtr[0] = static_cast<char>(0xF8 | (utf32Char >> 24));
            utf8StrPtr[1] = static_cast<char>(0x80 | ((utf32Char >> 18) & 0x3F));
            utf8StrPtr[2] = static_cast<char>(0x80 | ((utf32Char >> 12) & 0x3F));
            utf8StrPtr[3] = static_cast<char>(0x80 | ((utf32Char >> 6) & 0x3F));
            utf8StrPtr[4] = static_cast<char>(0x80 | (utf32Char & 0x3F));
            utf8StrPtr += 5;
        }
        else if (utf32Char < 0x80000000) {
            // 6 byte encoded character
            utf8StrPtr[0] = static_cast<char>(0xFC | (utf32Char >> 30));
            utf8StrPtr[1] = static_cast<char>(0x80 | ((utf32Char >> 24) & 0x3F));
            utf8StrPtr[2] = static_cast<char>(0x80 | ((utf32Char >> 18) & 0x3F));
            utf8StrPtr[3] = static_cast<char>(0x80 | ((utf32Char >> 12) & 0x3F));
            utf8StrPtr[4] = static_cast<char>(0x80 | ((utf32Char >> 6) & 0x3F));
            utf8StrPtr[5] = static_cast<char>(0x80 | (utf32Char & 0x3F));
            utf8StrPtr += 6;
        }
        
        ++utf32Str;
        --stringLength;
    }
    
    // Null terminate the utf8 string and return
    *utf8StrPtr = 0;
    return utf8StrBuffer;
}

WC_END_NAMESPACE
