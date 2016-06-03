#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType : public DataType {
public:
    virtual const char * name() const override;
};

WC_END_NAMESPACE