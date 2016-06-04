#include "WCIntDataType.hpp"

WC_BEGIN_NAMESPACE

const char * IntDataType::name() const {
    return "int";
}

bool IntDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const IntDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
