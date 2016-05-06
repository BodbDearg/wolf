#pragma once

namespace WCFileUtils {
    /** 
     * Reads a file as a null terminated utf8 c-string 
     */
    char * readFileAsUTF8String(const char * filePath);
}
