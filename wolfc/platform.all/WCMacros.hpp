#pragma once

/**
 * Begin namespace macro for the compiler.
 */
#define WC_BEGIN_NAMESPACE namespace wolfc {

/**
 * End namespace macro for the compiler.
 */
#define WC_END_NAMESPACE }

/**
 * Documentation macro. Clarify an empty function body is intentional.
 */
#define WC_EMPTY_FUNC_BODY()\
    do {} while (0)

/**
 * Get rid of unused parameter warnings from the compiler.
 */
#define WC_UNUSED_PARAM(param)\
    \
    (void) param

/**
 * Generate code to disallow copying and assign for the named type.
 */
#define WC_DISALLOW_COPY_AND_ASSIGN(ClassName)\
    \
    ClassName(const ClassName & other) = delete;\
    \
    ClassName & operator = (const ClassName & other) = delete;\
    \
    ClassName(const ClassName && other) = delete;\
    \
    ClassName && operator = (const ClassName && other) = delete;

/**
 * A macro inspired by the 'guard' statement from Apple's Swift language.
 * Checks the given expression (which must evaluate to a boolean) and returns out of the function if it evaluates to false.
 * An optional 2nd parameter can also be added to specify the return value on the expression failing.
 *
 * Macro params:
 * (1) The expression to check. If it evaluates to false then a return statement will execute.
 * (2) Optional parameter: the value or expression to return on the expression evaluating to false. 
 *     Neccessary for functions that Must return a value.
 */
#define WC_GUARD(...)\
    \
    WC_CALL_MACRO_OVERLOAD(__WC_GUARD_IMPL_, __VA_ARGS__)

/** 
 * The same as WC_GUARD, but will raise an assertion (if assertions are enabled) when the expression evaluates
 * to false. Useful for creating sanity checks that assert in debug but fail gracefully in release.
 */
#define WC_GUARD_ASSERT(...)\
    \
    WC_CALL_MACRO_OVERLOAD(__WC_GUARD_ASSERT_IMPL_, __VA_ARGS__)

/**
 * Make the debugger stop.
 */
#ifdef _MSC_VER
    
    #define WC_DEBUG_BREAK()\
        \
        do {\
            DebugBreak();\
        }   while (0)

#else

    #include <csignal>

    #define WC_DEBUG_BREAK()\
        \
        do {\
            raise(SIGINT);\
        }   while (0)

#endif

/**
 * Manual memory management.
 */
#define WC_SAFE_DELETE_NULL(ptr)\
    \
    do {\
        if (ptr) {\
            delete ptr;\
            ptr = nullptr;\
        }\
    }   while (0)

#define WC_SAFE_DELETE_ARRAY_NULL(arrayPtr)\
    \
    do {\
        if (arrayPtr) {\
            delete[] arrayPtr;\
            arrayPtr = nullptr;\
        }\
    }   while (0)

/**
 * Beginning of third party includes.
 * Disable warnings in third party includes.
 */
#ifdef __clang__
    // Clang: no better way to do this unfortunately (yet) other than one by one
    #define WC_THIRD_PARTY_INCLUDES_BEGIN\
        _Pragma("clang diagnostic push")\
        _Pragma("clang diagnostic ignored \"-Wall\"")\
        _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")\
        _Pragma("clang diagnostic ignored \"-Wunused-function\"")\
        _Pragma("clang diagnostic ignored \"-Wconversion\"")\
        _Pragma("clang diagnostic ignored \"-Wnewline-eof\"")\
        _Pragma("clang diagnostic ignored \"-Wnon-virtual-dtor\"")\
        _Pragma("clang diagnostic ignored \"-Wshadow\"")\
        _Pragma("clang diagnostic ignored \"-Wdocumentation\"")\
        _Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")
#else
    // Unhandled compiler
    #define WC_THIRD_PARTY_INCLUDES_BEGIN
#endif

/**
 * End of third party includes.
 * Re-enable all warnings previously disabled.
 */
#ifdef __clang__
    // Clang
    #define WC_THIRD_PARTY_INCLUDES_END\
        _Pragma("clang diagnostic pop")
#else
    // Unhandled compiler
    #define WC_THIRD_PARTY_INCLUDES_END
#endif

/**
 * Internal implementation stuff for the guard and guard assert macros.
 */
#define __WC_GUARD_IMPL_1(BooleanExpression)\
    \
    do {\
        if (!(BooleanExpression)) {\
            return;\
        }\
    } while (0)

#define __WC_GUARD_IMPL_2(BooleanExpression, ReturnValueExpression)\
    \
    do {\
        if (!(BooleanExpression)) {\
            return ReturnValueExpression;\
        }\
    } while (0)

#define __WC_GUARD_ASSERT_IMPL_1(BooleanExpression)\
    \
    do {\
        if (!(BooleanExpression)) {\
            WC_RAISE_ASSERTION("Guard condition evaluated to false!: " #BooleanExpression);\
            return;\
        }\
    } while (0)

#define __WC_GUARD_ASSERT_IMPL_2(BooleanExpression, ReturnValueExpression)\
    \
    do {\
        if (!(BooleanExpression)) {\
            WC_RAISE_ASSERTION("Guard condition evaluated to false!: " #BooleanExpression);\
            return ReturnValueExpression;\
        }\
    } while (0)

/**
 * Macro that counts the number of arguments in a varadic macro.
 * See: http://stackoverflow.com/questions/9183993/msvc-variadic-macro-expansion
 */
#define WC_COUNT_MACRO_ARGS(...)\
    __WC_COUNT_MACRO_ARGS_EXPAND_ARGS((__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define __WC_COUNT_MACRO_ARGS_EXPAND_ARGS(MACRO_ARGS)\
    __WC_COUNT_MACRO_ARGS_RETURN_ARG_COUNT MACRO_ARGS

#define __WC_COUNT_MACRO_ARGS_RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, MACRO_ARG_COUNT, ...)\
    MACRO_ARG_COUNT

/**
 * Macro that calls a numbered overload of a variadic macro.
 * See: http://stackoverflow.com/questions/9183993/msvc-variadic-macro-expansion
 */
#define WC_CALL_MACRO_OVERLOAD(MACRO_NAME, ...)\
    __WC_CALL_MACRO_OVERLOAD_GLUE(__WC_CALL_MACRO_OVERLOAD0(MACRO_NAME, WC_COUNT_MACRO_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define __WC_CALL_MACRO_OVERLOAD_GLUE(ARG_A, ARG_B)\
    ARG_A ARG_B

#define __WC_CALL_MACRO_OVERLOAD2(MACRO_NAME, MACRO_ARG_COUNT)\
    MACRO_NAME##MACRO_ARG_COUNT

#define __WC_CALL_MACRO_OVERLOAD1(MACRO_NAME, MACRO_ARG_COUNT)\
    __WC_CALL_MACRO_OVERLOAD2(MACRO_NAME, MACRO_ARG_COUNT)

#define __WC_CALL_MACRO_OVERLOAD0(MACRO_NAME, MACRO_ARG_COUNT)\
    __WC_CALL_MACRO_OVERLOAD1(MACRO_NAME, MACRO_ARG_COUNT)
