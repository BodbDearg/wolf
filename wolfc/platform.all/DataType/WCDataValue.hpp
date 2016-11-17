#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class DataType;

/* Struct holding an LLVM value and it's corresponding data type */
struct DataValue {
    DataValue() :
        value(nullptr),
        type(nullptr),
        requiresLoad(false)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    DataValue(llvm::Value * _value, DataType * _type, bool _requiresLoad) :
        value(_value),
        type(_type),
        requiresLoad(_requiresLoad)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    DataValue(const DataValue & other) = default;
    DataValue & operator = (const DataValue & other) = default;
    
    /* The llvm value representing the data */
    llvm::Value * value;
    
    /* The data type object for this type */
    DataType * type;
    
    /* If true the value requires a load first before being used. If not then it is already loaded */
    bool requiresLoad;
};

WC_END_NAMESPACE

