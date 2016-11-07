#pragma once

#include "WCDebugBreak.hpp"
#include "WCLogger.hpp"

/* Default setting for whether asserts are enabled or not */
#ifndef WC_ASSERT_ENABLED
    #ifndef NDEBUG
        #define WC_ASSERT_ENABLED 1
    #else
        #define WC_ASSERT_ENABLED 0
    #endif
#endif

/* Assert a condition. */
#if WC_ASSERT_ENABLED == 1
    #define WC_ASSERT(condition)\
        do {\
            if (!(condition)) {\
                WC_LOG_ERROR("[ASSERT FAIL!] File: %s Line: %i\nCondition: %s", __FILE__, __LINE__, #condition);\
                WC_DEBUG_BREAK();\
            }\
        }   while (0)
#else
    #define WC_ASSERT(condition)
#endif

/* Assert a condition and log output. */
#if WC_ASSERT_ENABLED == 1
    #define WC_ASSERT_LOG(condition, formatString, ...)\
        do {\
            if (!(condition)) {\
                WC_LOG_ERROR("[ASSERT FAIL!] File: %s Line: %i\nCondition: %s", __FILE__, __LINE__, #condition);\
                WC_LOG_ERROR("Assertion message follows: ");\
                WC_LOG_ERROR(formatString, #__VA_ARGS__);\
                WC_DEBUG_BREAK();\
            }\
        }   while (0)
#else
    #define WC_ASSERT_LOG(condition, formatString, ...)
#endif

/* Raise an assertion to indicate an erroneous or unexpected exceution state (no condition) */
#if WC_ASSERT_ENABLED == 1
    #define WC_RAISE_ASSERTION(formatString, ...)\
        do {\
            WC_LOG_ERROR("[ASSERTION RAISED!] File: %s Line: %i", __FILE__, __LINE__);\
            WC_LOG_ERROR("Assertion message follows: ");\
            WC_LOG_ERROR(formatString, #__VA_ARGS__);\
            WC_DEBUG_BREAK();\
        }   while (0)
#else
    #define WC_RAISE_ASSERTION(formatString, ...)
#endif
