#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

/* Base class for a data type */
class DataType {
public:
    virtual ~DataType();
    
    /* Give the name of the datatype, e.g 'int32' */
    virtual const char * name() const = 0;
};

WC_END_NAMESPACE
