#include "DataType.hpp"

#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"

WC_BEGIN_NAMESPACE

DataType::~DataType() {
    WC_EMPTY_FUNC_BODY();
}

bool DataType::isValid() const {
    return true;    // True for all types except 'Unknown'
}

bool DataType::isSized() const {
    return true;    // True for all types except 'Unknown' and 'Void'
}

bool DataType::isInteger() const {
    return false;   // False by default for everything except integer types
}

bool DataType::isSigned() const {
    return false;   // False by default for everything
}

bool DataType::isArray() const {
    return false;   // False by most types by default
}

bool DataType::isFunc() const {
    return false;   // False by most types by default
}

bool DataType::requiresStorage() const {
    return false;   // False by default for all types
}

WC_END_NAMESPACE
