#pragma once

#include "GenericIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all signed integer data types */
class GenericSignedIntDataType : public GenericIntDataType {
public:
    virtual bool isSigned() const final override;
};

WC_END_NAMESPACE
