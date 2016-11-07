#include "WCDebugBreak.hpp"

// Includes for Clang impl
#ifndef _MSC_VER
    #include <csignal>
#endif

WC_BEGIN_NAMESPACE

void debugBreak() {
    // Two compilers supported currently : MSVC and Clang
    #ifdef _MSC_VER
        // MSVC impl
        DebugBreak();
    #else
        // Clang impl
        raise(SIGINT);
    #endif
}

WC_END_NAMESPACE
