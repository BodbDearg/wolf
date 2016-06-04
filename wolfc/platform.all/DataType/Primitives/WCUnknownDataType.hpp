#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents an unknown type. Cannot be declared by the user. Used internally by the
 * compiler in the case of an error.
 */
class UnknownDataType : public DataType {
public:
    virtual const char * name() const override;
    
    virtual bool equals(const DataType & other) const override;
};

WC_END_NAMESPACE
