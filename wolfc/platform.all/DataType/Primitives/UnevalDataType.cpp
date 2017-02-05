#include "UnevalDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "?";

void UnevalDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId UnevalDataType::getTypeId() const {
    return DataTypeId::kInvalid;
}

const std::string & UnevalDataType::name() const {
    return kName;
}

bool UnevalDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UnevalDataType*>(&other) != nullptr;
}

bool UnevalDataType::isValid() const {
    return false;
}

bool UnevalDataType::isSized() const {
    return false;
}

WC_END_NAMESPACE
