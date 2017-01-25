#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all integer data types */
class GenericIntDataType : public DataType {
public:
    virtual bool isInteger() const final override;
};

WC_END_NAMESPACE
