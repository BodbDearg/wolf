//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* An interface that looping constructs (for/while/repeat etc.) must use */
class IRepeatableStmnt : public IStmnt {
public:
    virtual ~IRepeatableStmnt() = default;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
