//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

/* Default setting for whether debug breaking is enabled or not */
#ifndef WC_DEBUG_BREAK_ENABLED
    #ifndef NDEBUG
        #define WC_DEBUG_BREAK_ENABLED 1
    #else
        #define WC_DEBUG_BREAK_ENABLED 0
    #endif
#endif

/* The macro for doing a debug break */
#if WC_DEBUG_BREAK_ENABLED == 1
    #define WC_DEBUG_BREAK() WC_NAMESPACE_NAME::debugBreak()
#else
    #define WC_DEBUG_BREAK()
#endif

WC_BEGIN_NAMESPACE
    /* Breaks the debugger if possible */
    void debugBreak();
WC_END_NAMESPACE
