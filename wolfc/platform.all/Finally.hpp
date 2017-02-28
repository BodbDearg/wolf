//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Assert.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <functional>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* Object which invokes a lambda on destruction. Useful for RAII type patterns. */
class Finally {
public:
    Finally(const std::function<void ()> & callback) :
        mCallback(callback)
    {
        WC_ASSERT(mCallback);
    }
    
    ~Finally() {
        mCallback();
    }
    
private:
    std::function<void ()>  mCallback;
};

WC_END_NAMESPACE
