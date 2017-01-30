#include "UnknownDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "?";

void UnknownDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId UnknownDataType::getTypeId() const {
    return DataTypeId::kUnknown;
}

const std::string & UnknownDataType::name() const {
    return kName;
}

bool UnknownDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UnknownDataType*>(&other) != nullptr;
}

bool UnknownDataType::isValid() const {
    return false;
}

bool UnknownDataType::isSized() const {
    return false;
}

bool UnknownDataType::isUnknown() const {
    return false;
}

WC_END_NAMESPACE
