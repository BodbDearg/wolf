//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "UndefinedDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "<undefined>";

void UndefinedDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId UndefinedDataType::getTypeId() const {
    return DataTypeId::kUndefined;
}

const std::string & UndefinedDataType::name() const {
    return kName;
}

bool UndefinedDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UndefinedDataType*>(&other) != nullptr;
}

bool UndefinedDataType::isValid() const {
    return false;
}

bool UndefinedDataType::isSized() const {
    return false;
}

WC_END_NAMESPACE
