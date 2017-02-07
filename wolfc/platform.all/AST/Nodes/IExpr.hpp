#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* An interface that all expressions must implement. */
class IExpr {
public:
    /* Nothing here at the moment. There used to be stuff but it was moved out of the AST */
    virtual ~IExpr() = default;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
