#include "WCBoolDataType.hpp"

WC_BEGIN_NAMESPACE

const char * BoolDataType::name() const {
    return "bool";
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
