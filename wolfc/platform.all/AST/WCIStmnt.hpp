#pragma once

#include "WCIBasicCodegenNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;

/* An interface that all statements must implement. */
class IStmnt : public IBasicCodegenNode {
public:
    /**
     * Tells if all codepaths within the statement produce an unconditional return statement.
     * This is useful for functions that return a value to verify that all codepaths actually
     * return something...
     */
    virtual bool allCodepathsHaveUncondRet() const = 0;
};

WC_END_NAMESPACE
