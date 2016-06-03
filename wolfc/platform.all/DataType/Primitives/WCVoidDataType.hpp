#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Similar to 'unknown' in many ways but this type is exposed to the user. A data type that represents no
 * data type, or no data.
 */
class VoidDataType : public DataType {
public:
    virtual const char * name() const override;
};

WC_END_NAMESPACE