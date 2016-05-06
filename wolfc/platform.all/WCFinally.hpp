#pragma once

#include "WCMacros.hpp"
#include <functional>

/**
 * Object which invokes a lambda on destruction. Useful for RAII type patterns.
 */
class WCFinally {
public:
    WCFinally(const std::function<void ()> & callback) : mCallback(callback) {
        WC_ASSERT(mCallback);
    }
    
    ~WCFinally() {
        mCallback();
    }
    
private:
    std::function<void ()>  mCallback;
};
