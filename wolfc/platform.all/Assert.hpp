#pragma once

#include "DebugBreak.hpp"
#include "Logger.hpp"
#include "Macros.hpp"

/* Default setting for whether asserts are enabled or not */
#ifndef WC_ASSERT_ENABLED
    #ifndef NDEBUG
        #define WC_ASSERT_ENABLED 1
    #else
        #define WC_ASSERT_ENABLED 0
    #endif
#endif

/**
 * Assert a condition and log an error message (and breaking when debugging) if failed.
 *
 * The param is the condition to assert.
 * Asserts are only compiled in if they are enabled in the current build.
 */
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

/**
 * Assert a condition and log an error message (and breaking when debugging) if failed.
 *
 * The 1st param is the condition to assert.
 * The 2nd param is a printf format string for the assertion failed message.
 * Subsequent arguments are printf style varargs for assertion message placeholders.
 *
 * Asserts are only compiled in if they are enabled in the current build.
 */
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

/**
 * Raise an assertion as having failed to indicate an erroneous or unexpected 
 * exceution state. This flavor of assert always asserts and has no condition.
 *
 * The 1st param is a printf format string for the assertion failed message.
 * Subsequent arguments are printf style varargs for assertion message placeholders.
 *
 * Assertion failed messages are only compiled in if asserts are enabled in the current build.
 */
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

/** 
 * The same as WC_GUARD found in the macros header, but will raise an assertion (if assertions are enabled) 
 * when the expression evaluates to false. 
 * Useful for creating sanity checks that assert in debug but fail gracefully in release.
 */
#define WC_GUARD_ASSERT(...)\
    WC_CALL_MACRO_OVERLOAD(__WC_GUARD_ASSERT_IMPL_, __VA_ARGS__)

/**
 * Macro for an expression/operation that is asserted. 
 * The given expression (function call etc.) must evaluate to a boolean. 
 * When asserts are enabled the result of the operation will be asserted upon. 
 * When asserts are disabled the operation is performed normally but without the assert checks.
 *
 * The 1st param is the expression/operation to assert upon.
 * The 2nd param is a printf format string for the assertion failed message.
 * Subsequent arguments are printf style varargs for assertion message placeholders.
 *
 * Asserts are only compiled in if they are enabled in the current build but the 
 * operation given will be performed in ANY build.
 */
#define WC_ASSERTED_OP(...)\
    WC_CALL_MACRO_OVERLOAD(__WC_ASSERTED_OP_IMPL_, __VA_ARGS__)

/* Internal implementation stuff for the guard assert macro */
#define __WC_GUARD_ASSERT_IMPL_1(BooleanExpression)\
    do {\
        if (!(BooleanExpression)) {\
            WC_RAISE_ASSERTION("Guard condition evaluated to false!: %s" #BooleanExpression);\
            return;\
        }\
    } while (0)

#define __WC_GUARD_ASSERT_IMPL_2(BooleanExpression, ReturnValueExpression)\
    do {\
        if (!(BooleanExpression)) {\
            WC_RAISE_ASSERTION("Guard condition evaluated to false!: %s" #BooleanExpression);\
            return ReturnValueExpression;\
        }\
    } while (0)

/* Internal implementation stuff for the asserted op macro */
#define __WC_ASSERTED_OP_IMPL_1(BooleanExpr)\
    do {\
        if (!(BooleanExpr)) {\
            WC_RAISE_ASSERTION("Operation which should have succeeded failed!: %s", #BooleanExpr);\
        }\
    } while (0)

#define __WC_ASSERTED_OP_IMPL_2_OR_MORE(BooleanExpr, ...)\
    do {\
        if (!(BooleanExpr)) {\
            WC_LOG_ERROR("Operation which should have succeeded failed!: %s", #BooleanExpr);\
            WC_RAISE_ASSERTION(__VA_ARGS__);\
        }\
    } while (0)

#define __WC_ASSERTED_OP_IMPL_2(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_3(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_4(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_5(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_6(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_7(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_8(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_9(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_10(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_11(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_12(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_13(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_14(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_15(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
#define __WC_ASSERTED_OP_IMPL_16(...) __WC_ASSERTED_OP_IMPL_2_OR_MORE(__VA_ARGS__)
