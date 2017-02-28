//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "InvalidDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "<invalid>";

void InvalidDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId InvalidDataType::getTypeId() const {
    return DataTypeId::kInvalid;
}

const std::string & InvalidDataType::name() const {
    return kName;
}

bool InvalidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const InvalidDataType*>(&other) != nullptr;
}

bool InvalidDataType::isValid() const {
    return false;
}

bool InvalidDataType::isSized() const {
    return false;
}

WC_END_NAMESPACE
