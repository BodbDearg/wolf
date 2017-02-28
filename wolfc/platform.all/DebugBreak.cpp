//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "DebugBreak.hpp"

// Includes for Clang impl
#ifdef _MSC_VER
    #include <Windows.h>
#else
    // Clang and other compilers
    #include <csignal>
#endif

WC_BEGIN_NAMESPACE

void debugBreak() {
    #ifdef _MSC_VER
        // Visual C++ impl
        DebugBreak();
    #else
        // Clang and other compilers
        raise(SIGINT);
    #endif
}

WC_END_NAMESPACE
