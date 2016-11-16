#pragma once

#include "WCAssert.hpp"
#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

/**
 * A wrapper for an object which is stored by value but which does not need to be initialized immediately.
 * Can be used to store objects with the same efficiency as value semantics but without the restriction
 * that the object must be constructed immediately.
 */
template <typename T>
class LazyInit {
public:
    inline LazyInit() : mWasInitialized(false) {
        // Don't initialize the object memory
    }
    
    inline ~LazyInit() {
        if (mWasInitialized) {
            reinterpret_cast<T*>(mObjMem)->~T();
        }
    }
    
    inline T & operator * () {
        WC_ASSERT(mWasInitialized);
        return *reinterpret_cast<T*>(mObjMem);
    }
    
    inline T * operator->() {
        WC_ASSERT(mWasInitialized);
        return reinterpret_cast<T*>(mObjMem);
    }
    
    inline T & get() {
        WC_ASSERT(mWasInitialized);
        return *reinterpret_cast<T*>(mObjMem);
    }
    
    inline bool wasInitialized() const {
        return mWasInitialized;
    }
    
    template <typename... Types>
    inline void init(Types & ... params) {
        WC_ASSERT(!mWasInitialized);
        new (mObjMem) T(params...);
        mWasInitialized = true;
    }
    
private:
    uint8_t     mObjMem[sizeof(T)];
    bool        mWasInitialized;
};

WC_END_NAMESPACE
