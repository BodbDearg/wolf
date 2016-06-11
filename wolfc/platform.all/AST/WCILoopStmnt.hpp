#pragma once

#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

/* An interface that looping constructs (for/while/repeat etc.) must use */
class ILoopStmnt : public IStmnt {
public:
    virtual ~ILoopStmnt() = default;
    
    /**
     * Returns the block that must be jumped to in order for the loop to restart.
     * This is the block that the 'next' statement will jump to.
     */
    virtual llvm::BasicBlock * getStartBlock() = 0;
    
    /**
     * Returns the block that must be jumped to in order for the loop to end.
     * This is the block that the 'break' statement will jump to.
     */
    virtual llvm::BasicBlock * getEndBlock() = 0;
};

WC_END_NAMESPACE
