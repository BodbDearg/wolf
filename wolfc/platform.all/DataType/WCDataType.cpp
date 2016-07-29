#include "WCDataType.hpp"

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

bool DataType::isUnknown() const {
    return false;   // False for all types except 'Unknown'
}

bool DataType::isVoid() const {
    return false;   // False for all types except 'void'
}

bool DataType::isBool() const {
    return false;   // False for all types except 'bool'
}

WC_END_NAMESPACE
