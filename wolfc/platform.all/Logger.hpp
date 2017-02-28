//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <functional>
    #include <set>
    #include <map>
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

/* Default setting for whether logging is enabled or not */
#ifndef WC_ENABLE_LOGGING
    #ifndef NDEBUG
        #define WC_ENABLE_LOGGING 1
    #else
        #define WC_ENABLE_LOGGING 0
    #endif
#endif

/* Logging macros and a class which controls what gets logged. */
#ifdef WC_ENABLE_LOGGING

    #define WC_LOG(categoryString, formatString, ...)\
        do {\
            WCLogger::log(categoryString, formatString,  ##__VA_ARGS__);\
        }   while (0)

    #define WC_LOG_INFO(formatString, ...)\
        do {\
            WCLogger::logInfo(formatString,  ##__VA_ARGS__);\
        }   while (0)

    #define WC_LOG_WARNING(formatString, ...)\
        do {\
            WCLogger::logWarning(formatString,  ##__VA_ARGS__);\
        }   while (0)

    #define WC_LOG_ERROR(formatString, ...)\
        do {\
            WCLogger::logError(formatString,  ##__VA_ARGS__);\
        }   while (0)

#else   // #ifdef WC_ENABLE_LOGGING

    #define WC_LOG(categoryString, formatString, ...)
    #define WC_LOG_INFO(formatString, ...)
    #define WC_LOG_WARNING(formatString, ...)
    #define WC_LOG_ERROR(formatString, ...)

#endif  // #ifdef WC_ENABLE_LOGGING


#ifdef WC_ENABLE_LOGGING

class WCLogger {
public:
    /* Type for a hook function which receives stuff to be logged. */
    typedef std::function<void (const char * loggedMessage)> LogHookFunction;

    /* Logging categories. */
    static const std::string kInfoCategory;
    static const std::string kWarningCategory;
    static const std::string kErrorCategory;
    
    /* The logging functions themselves */
    static void log(const char * category, const char * formatString, ...);
    
    static void logInfo(const char * formatString, ...);
    
    static void logWarning(const char * formatString, ...);
    
    static void logError(const char * formatString, ...);
    
    /* Tell/set if a logging category is enabled. */
    static bool isLoggingCategoryEnabled(const std::string & category);
    
    static void setLoggingCategoryEnabled(const std::string & category, bool enable);
    
    /**
     * Add a function which will receive strings being logged when they are logged.
     * Can use it to direct log output to additional sources other than the standard platform output.
     */
    static void addLogHook(const std::string & hookName, const LogHookFunction & onLogCallback);
    
    /* Remove afforementioned log hook. */
    static void removeLogHook(const std::string & hookName);
    
private:
    /* Does the actual work of logging. */
    static void logImpl(const char * stringToLog, bool error);
    
    static void sendLogMessageToHooks(const char * message);

    static std::set<std::string>                    sEnabledLoggingCategories;
    static std::map<std::string, LogHookFunction>   sLogHookFunctions;
};

#endif  // #ifdef WC_ENABLE_LOGGING
