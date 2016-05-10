#pragma once

#include "WCMacros.hpp"
#include <functional>

WC_BEGIN_NAMESPACE

/**
 * Object which invokes a lambda on destruction. Useful for RAII type patterns.
 */
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
