#pragma once

#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

/**
 * Interface for a statement that defers it's actual code generation until later.
 * Deferred code generation is needed for statements that modify control flow like next, break, return etc.
 * because they will need to refer to basic blocks that may not have been code generated yet at that visitation 
 * point in the AST tree.
 *
 * For this type of statement, 'codegenStmnt' will just generate stuff like basic blocks etc. and
 * 'deferredCodegenStmnt' will do the actual work of code generation.
 */
class IDeferredCodegenStmnt : public IStmnt {
public:
    /**
     * Do the actual work of generating the llvm code for this statement.
     * Returns true on success or false on failure.
     */
    virtual bool deferredCodegenStmnt(CodegenCtx & cgCtx) = 0;
};

WC_END_NAMESPACE
