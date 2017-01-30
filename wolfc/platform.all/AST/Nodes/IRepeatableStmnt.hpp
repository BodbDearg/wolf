#pragma once

#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* An interface that looping constructs (for/while/repeat etc.) must use */
class IRepeatableStmnt : public IStmnt {
public:
    virtual ~IRepeatableStmnt() = default;
    
#warning FIXME - Codegen
#if 0
    /**
     * Returns the block that must be jumped to in order for the loop to skip to the next iteration.
     * This is the block that the 'next' statement will jump to.
     */
    virtual llvm::BasicBlock * getNextStmntTargetBlock() = 0;
    
    /**
     * Returns the block that must be jumped to in order for the loop to be broken out of.
     * This is the block that the 'break' statement will jump to.
     */
    virtual llvm::BasicBlock * getBreakStmntTargetBlock() = 0;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
