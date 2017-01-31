#include "StrDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "string";

void StrDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId StrDataType::getTypeId() const {
    return DataTypeId::kStr;
}

const std::string & StrDataType::name() const {
    return kName;
}

bool StrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const StrDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
