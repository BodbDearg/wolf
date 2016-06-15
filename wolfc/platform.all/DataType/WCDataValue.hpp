#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class DataType;

/* Struct holding an LLVM value and it's corresponding data type */
struct DataValue {
    DataValue() : value(nullptr), type(nullptr) {
        WC_EMPTY_FUNC_BODY();
    }
    
    DataValue(llvm::Value * _value, const DataType * _type) : value(_value), type(_type) {
        WC_EMPTY_FUNC_BODY();
    }
    
    DataValue(const DataValue & other) : value(other.value), type(other.type) {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* The llvm value representing the data */
    llvm::Value * value;
    
    /* The data type object for this type */
    const DataType * type;
};

WC_END_NAMESPACE

