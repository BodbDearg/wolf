#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

/* An interface that all expressions must implement. */
class IExpr {
public:
    virtual ~IExpr() = default;
    
    /* Return the data type that this expression results in after evaluation. */
    virtual const DataType & dataType() const = 0;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
