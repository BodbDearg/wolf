//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "NullptrDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "nullptr";

void NullptrDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId NullptrDataType::getTypeId() const {
    return DataTypeId::kAny;
}

const std::string & NullptrDataType::name() const {
    return kName;
}

bool NullptrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const NullptrDataType*>(&other) != nullptr;
}

bool NullptrDataType::isValid() const {
    return true;
}

bool NullptrDataType::isSized() const {
    return true;
}

WC_END_NAMESPACE
