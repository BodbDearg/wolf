#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Signed integer data type. */
class IntDataType : public DataType {
public:
    virtual const char * name() const override;
    
    virtual bool equals(const DataType & other) const override;
};

WC_END_NAMESPACE
