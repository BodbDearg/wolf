//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all integer data types */
class GenericIntDataType : public DataType {
public:
    virtual bool isInteger() const final override;
};

WC_END_NAMESPACE
