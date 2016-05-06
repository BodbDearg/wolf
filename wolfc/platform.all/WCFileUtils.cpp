#include "WCFileUtils.hpp"
#include "WCStringUtils.hpp"
#include "WCFinally.hpp"
#include <cstdio>

char * WCFileUtils::readUTF8TextFileAsUTF8String(const char * filePath, size_t & fileSizeInBytes) {
    // Open the file
    fileSizeInBytes = 0;
    std::FILE * file = std::fopen(filePath, "rb");
    
    if (!file) {
        return nullptr;
    }
    
    WCFinally closeFile([&](){
        std::fclose(file);
    });
    
    // Figure out the size of the file
    if (std::fseek(file, 0, SEEK_END) != 0) {
        return nullptr;
    }
    
    long fileSize = std::ftell(file);
    
    if (fileSize < 0) {
        return nullptr;
    }
    
    if (std::fseek(file, 0, SEEK_SET) != 0) {
        return nullptr;
    }
    
    // Alloc char buffer and read
    char * charBuffer = new char[fileSize + 1];
    
    if (!charBuffer) {
        return nullptr;
    }
    
    if (fileSize > 0) {
        if (std::fread(charBuffer, fileSize, 1, file) != 1) {
            delete[] charBuffer;
            return nullptr;
        }
    }
    
    // Null terminate buffer and return
    charBuffer[fileSize] = 0;
    fileSizeInBytes = fileSize;
    return charBuffer;
}

char32_t * WCFileUtils::readUTF8TextFileAsUTF32String(const char * filePath) {
    size_t fileSizeInBytes = 0;
    std::unique_ptr<char[]> utf8Str(readUTF8TextFileAsUTF8String(filePath, fileSizeInBytes));
    
    if (utf8Str) {
        return WCStringUtils::convertUtf8ToUtf32(utf8Str.get(), fileSizeInBytes);
    }
    
    return nullptr;
}
