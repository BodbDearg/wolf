//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace FileUtils {
    /* Reads into a null terminated c-string. Returns the size of the file in the given param. */
    char * readTextFileAsCString(const char * filePath, size_t & fileSizeInBytes);
    
    /* Same as other read function, but without output file size given. */
    char * readTextFileAsCString(const char * filePath);
}

WC_END_NAMESPACE
