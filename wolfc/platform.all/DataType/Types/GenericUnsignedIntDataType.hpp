//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "GenericIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all unsigned integer data types */
class GenericUnsignedIntDataType : public GenericIntDataType {
public:
    virtual bool isSigned() const final override;
};

WC_END_NAMESPACE
