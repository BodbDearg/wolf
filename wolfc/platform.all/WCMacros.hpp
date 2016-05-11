#pragma once

#include <cassert>

/**
 * Begin namespace macro for the compiler.
 */
#define WC_BEGIN_NAMESPACE namespace wolfc {

/**
 * End namespace macro for the compiler.
 */
#define WC_END_NAMESPACE }

/**
 * Assert macro.
 */
#define WC_ASSERT(cond)\
    assert((cond))

/**
 * Documentation macro. Specify an empty function body.
 */
#define WC_EMPTY_FUNC_BODY()

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
