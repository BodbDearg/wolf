#include "WCVoidDataType.hpp"

WC_BEGIN_NAMESPACE

const char * VoidDataType::name() const {
    return "void";
}

bool VoidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const VoidDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
