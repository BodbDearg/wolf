#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;

/* An interface that all statements must implement. */
class IStmnt {
public:
    virtual ~IStmnt() = default;
    
    /* Generate the llvm code for this statement. Returns true on success or false on failure. */
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
};

WC_END_NAMESPACE
