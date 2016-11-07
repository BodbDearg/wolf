#include "WCFileUtils.hpp"

#include "WCFinally.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdio>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

char * FileUtils::readTextFileAsCString(const char * filePath, size_t & fileSizeInBytes) {
    // Open the file
    fileSizeInBytes = 0;
    std::FILE * file = std::fopen(filePath, "rb");
    WC_GUARD(file, nullptr);
    
    // This cleans up when we are done
    Finally closeFile([&](){
        std::fclose(file);
    });
    
    // Figure out the size of the file
    WC_GUARD(std::fseek(file, 0, SEEK_END) == 0, nullptr);
    long fileSize = std::ftell(file);
    WC_GUARD(fileSize >= 0, nullptr);
    WC_GUARD(std::fseek(file, 0, SEEK_SET) == 0, nullptr);
    
    // Alloc char buffer and read
    char * charBuffer = new char[static_cast<size_t>(fileSize) + 1];
    WC_GUARD(charBuffer, nullptr);
    
    if (fileSize > 0) {
        if (std::fread(charBuffer, static_cast<size_t>(fileSize), 1, file) != 1) {
            delete[] charBuffer;
            return nullptr;
        }
    }
    
    // Null terminate buffer and return
    charBuffer[fileSize] = 0;
    fileSizeInBytes = static_cast<size_t>(fileSize);
    return charBuffer;
}

char * FileUtils::readTextFileAsCString(const char * filePath) {
    size_t unusedOutputFileSize = 0;
    return readTextFileAsCString(filePath, unusedOutputFileSize);
}

WC_END_NAMESPACE
