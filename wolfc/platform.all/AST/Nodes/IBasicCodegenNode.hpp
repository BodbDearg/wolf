//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/**
 * A node that implements a basic codegen method. 
 * Intended for statements that do not evaluate to a value (non expressions).
 */
class IBasicCodegenNode {
public:
    virtual ~IBasicCodegenNode() = default;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
