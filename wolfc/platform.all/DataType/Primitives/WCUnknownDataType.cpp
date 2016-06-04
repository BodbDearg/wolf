#include "WCUnknownDataType.hpp"

WC_BEGIN_NAMESPACE

const char * UnknownDataType::name() const {
    return "unknown";
}

bool UnknownDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UnknownDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
