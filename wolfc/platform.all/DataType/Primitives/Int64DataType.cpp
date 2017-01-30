#include "Int64DataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "int64";

void Int64DataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId Int64DataType::getTypeId() const {
    return DataTypeId::kInt64;
}

const std::string & Int64DataType::name() const {
    return kName;
}

bool Int64DataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const Int64DataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
