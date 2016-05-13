#pragma once

#include "WCMacros.hpp"
#include <cstddef>

WC_BEGIN_NAMESPACE

namespace FileUtils {
    /* Reads a file encoded as utf8 as a null terminated utf8 c-string */
    char * readUTF8TextFileAsUTF8String(const char * filePath, size_t & fileSizeInBytes);
    
    /* Reads a file encoded as utf8 as a null terminated utf32 c-string */
    char32_t * readUTF8TextFileAsUTF32String(const char * filePath);
}

WC_END_NAMESPACE
