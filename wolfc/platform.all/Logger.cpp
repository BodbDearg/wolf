//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Logger.hpp"

#ifdef WC_ENABLE_LOGGING

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <array>
    #include <cstdarg>
WC_THIRD_PARTY_INCLUDES_END

static const unsigned kMessageBufferSize = 0xFFFF;
static const unsigned kMaxMessageLength = kMessageBufferSize - 1;   // -1 for NULL terminator

const std::string WCLogger::kInfoCategory = "INFO";
const std::string WCLogger::kWarningCategory = "WARNING";
const std::string WCLogger::kErrorCategory = "ERROR";

std::set<std::string> WCLogger::sEnabledLoggingCategories = {
    kInfoCategory,
    kWarningCategory,
    kErrorCategory
};

std::map<std::string, WCLogger::LogHookFunction> WCLogger::sLogHookFunctions;

static void formatMessage(const char * category,
                          const char * formatString,
                          va_list varArgsList,
                          std::array<char, kMessageBufferSize> & messageBuffer)
{
    WC_GUARD(category && formatString);
    
    // Firstly put the category chars into the buffer plus a space at the end
    static_assert(kMessageBufferSize >= 128, "Buffer size too small for any meaningful messages!");
    
    size_t bufferPosition = 0;
    messageBuffer[bufferPosition] = '[';
    ++bufferPosition;
    
    {
        const char * copyFrom = category;
        
        while (bufferPosition + 1 < kMaxMessageLength && copyFrom[0]) {
            messageBuffer[bufferPosition] = copyFrom[0];
            ++bufferPosition;
            ++copyFrom;
        }
        
        if (bufferPosition < kMaxMessageLength) {
            messageBuffer[bufferPosition] = ']';
            ++bufferPosition;
        }
        
        if (bufferPosition < kMaxMessageLength) {
            messageBuffer[bufferPosition] = ' ';
            ++bufferPosition;
        }
    }
    
    messageBuffer[bufferPosition] = 0;  // Okay, good. Now NULL terminate
    
    // Now do varargs printf to the buffer
    vsnprintf(messageBuffer.data() + bufferPosition,
              kMaxMessageLength - bufferPosition,
              formatString,
              varArgsList);
}

void WCLogger::log(const char * category, const char * formatString, ...) {
    if (!category || sEnabledLoggingCategories.count(category) == 0) {
        return;     // Logging disabled for this category
    }
    
    std::array<char, kMessageBufferSize> messageBuffer;
    
    va_list varArgsList;
    va_start(varArgsList, formatString);
    formatMessage(category, formatString, varArgsList, messageBuffer);
    va_end(varArgsList);
    
    logImpl(messageBuffer.data(), false);
    sendLogMessageToHooks(messageBuffer.data());
}

void WCLogger::logInfo(const char * formatString, ...) {
    if (sEnabledLoggingCategories.count(kInfoCategory) == 0) {
        return;     // Logging disabled for this category
    }
    
    std::array<char, kMessageBufferSize> messageBuffer;
    
    va_list varArgsList;
    va_start(varArgsList, formatString);
    formatMessage(kInfoCategory.c_str(), formatString, varArgsList, messageBuffer);
    va_end(varArgsList);
    
    logImpl(messageBuffer.data(), false);
    sendLogMessageToHooks(messageBuffer.data());
}

void WCLogger::logWarning(const char * formatString, ...) {
    if (sEnabledLoggingCategories.count(kWarningCategory) == 0) {
        return;     // Logging disabled for this category
    }
    
    std::array<char, kMessageBufferSize> messageBuffer;
    
    va_list varArgsList;
    va_start(varArgsList, formatString);
    formatMessage(kWarningCategory.c_str(), formatString, varArgsList, messageBuffer);
    va_end(varArgsList);
    
    logImpl(messageBuffer.data(), false);
    sendLogMessageToHooks(messageBuffer.data());
}

void WCLogger::logError(const char * formatString, ...) {
    if (sEnabledLoggingCategories.count(kErrorCategory) == 0) {
        return;     // Logging disabled for this category
    }
    
    std::array<char, kMessageBufferSize> messageBuffer;
    
    va_list varArgsList;
    va_start(varArgsList, formatString);
    formatMessage(kErrorCategory.c_str(), formatString, varArgsList, messageBuffer);
    va_end(varArgsList);
    
    logImpl(messageBuffer.data(), true);
    sendLogMessageToHooks(messageBuffer.data());
}

bool WCLogger::isLoggingCategoryEnabled(const std::string & category) {
    return sEnabledLoggingCategories.count(category) > 0;
}

void WCLogger::setLoggingCategoryEnabled(const std::string & category, bool enable) {
    if (enable) {
        sEnabledLoggingCategories.insert(category);
    }
    else {
        sEnabledLoggingCategories.erase(category);
    }
}

void WCLogger::addLogHook(const std::string & hookName, const LogHookFunction & onLogCallback) {
    sLogHookFunctions[hookName] = onLogCallback;
}

void WCLogger::removeLogHook(const std::string & hookName) {
    auto iter = sLogHookFunctions.find(hookName);
    
    if (iter != sLogHookFunctions.end()) {
        sLogHookFunctions.erase(iter);
    }
}

void WCLogger::logImpl(const char * stringToLog, bool error) {
    if (error) {
        std::fprintf(stderr, "%s", stringToLog);
    }
    else {
        std::fprintf(stdout, "%s", stringToLog);
    }
}

void WCLogger::sendLogMessageToHooks(const char * message) {
    for (const auto & pair : sLogHookFunctions) {
        pair.second(message);
    }
}

#endif  // #ifdef WC_ENABLE_LOGGING
