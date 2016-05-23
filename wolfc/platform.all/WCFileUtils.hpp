#pragma once

#include "WCMacros.hpp"
#include <cstddef>

WC_BEGIN_NAMESPACE

namespace FileUtils {
    /* Reads into a null terminated c-string. Returns the size of the file in the given param. */
    char * readTextFileAsCString(const char * filePath, size_t & fileSizeInBytes);
    
    /* Same as other read function, but without output file size given. */
    char * readTextFileAsCString(const char * filePath);
}

WC_END_NAMESPACE
