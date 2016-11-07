#include "WCIExpr.hpp"

#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

bool IExpr::requiresStorage() const {
    return false;       // False for all types by default
}

llvm::Value * IExpr::getStorage() const {
    return nullptr;     // None for most types by default
}

void IExpr::setStorage(llvm::Value & storage) {
    // Not implemented for most types by default!
    WC_UNUSED_PARAM(storage);
    WC_RAISE_ASSERTION("This type does not require storage, not implemented!");
}

WC_END_NAMESPACE
