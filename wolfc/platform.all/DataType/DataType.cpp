//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "DataType.hpp"

#include "AST/Nodes/ASTNode.hpp"
#include "Assert.hpp"

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
    return false;   // False for all types by default
}

bool DataType::isPtr() const {
    return false;   // False for everything except pointers
}

bool DataType::isArray() const {
    return false;   // False for everything except arrays
}

bool DataType::isFunc() const {
    return false;   // False for everything except functions
}

bool DataType::requiresStorage() const {
    return false;   // False by default for all types
}

WC_END_NAMESPACE
