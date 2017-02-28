#include "VoidDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "void";

void VoidDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId VoidDataType::getTypeId() const {
    return DataTypeId::kVoid;
}

const std::string & VoidDataType::name() const {
    return kName;
}

bool VoidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const VoidDataType*>(&other) != nullptr;
}

bool VoidDataType::isSized() const {
    return false;
}

WC_END_NAMESPACE
