#pragma once

#include "WCIBasicCodegenNode.hpp"

WC_BEGIN_NAMESPACE

class CodegenCtx;

/* An interface that all statements must implement. */
class IStmnt : public IBasicCodegenNode {
public:
    // TODO: should we remove this interface eventually? Or will it be useful some day?
};

WC_END_NAMESPACE
