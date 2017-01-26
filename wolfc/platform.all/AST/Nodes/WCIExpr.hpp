#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

/* An interface that all expressions must implement. */
class IExpr {
public:
    virtual ~IExpr() = default;
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() const = 0;
    
    /* Tells if it's possible for this expression to be evaluated as a constant */
    virtual bool isConstExpr() const = 0;
    
    /* Return the data type that this expression results in after evaluation. */
    virtual const DataType & dataType() const = 0;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
