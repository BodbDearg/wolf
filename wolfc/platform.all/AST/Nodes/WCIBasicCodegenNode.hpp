#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/**
 * A node that implements a basic codegen method. 
 * Intended for statements that do not evaluate to a value (non expressions).
 */
class IBasicCodegenNode {
public:
    virtual ~IBasicCodegenNode() = default;
    
#warning FIXME - Codegen
#if 0
    /* Generate the llvm code for this statement. Returns true on success or false on failure. */
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
