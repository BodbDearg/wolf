#include "WCBoolDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "bool";

void BoolDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId BoolDataType::getTypeId() const {
    return DataTypeId::kBool;
}

const std::string & BoolDataType::name() const {
    return kName;
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

WC_END_NAMESPACE
