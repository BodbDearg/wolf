//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AnyDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "any";

void AnyDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId AnyDataType::getTypeId() const {
    return DataTypeId::kAny;
}

const std::string & AnyDataType::name() const {
    return kName;
}

bool AnyDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const AnyDataType*>(&other) != nullptr;
}

bool AnyDataType::isValid() const {
    // The 'any' data type is not valid because it is ambiguous- can be anything!
    return false;
}

bool AnyDataType::isSized() const {
    // The 'any' data type always refers to a sized data type
    return true;
}

WC_END_NAMESPACE
