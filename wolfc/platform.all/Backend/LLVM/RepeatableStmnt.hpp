#pragma once

#include "Macros.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
    class IRepeatableStmnt;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Holds information about a repeatable statement such as a 'loop' statement or 'while' statement */
class RepeatableStmnt {
public:
    /* Creates the repeatable statement definition. */
    RepeatableStmnt(const AST::ASTNode & astNode,
                    const AST::IRepeatableStmnt & astRepeatableStmnt)
    :
        mASTNode(astNode),
        mASTRepeatableStmnt(astRepeatableStmnt)
    {
        
    }
    
    /* The AST node for the repeatable statement */
    const AST::ASTNode & mASTNode;
    
    /* The AST node for the repeatable statement */
    const AST::IRepeatableStmnt & mASTRepeatableStmnt;
    
    /* The llvm basic block for where a 'next' statement jumps to */
    llvm::BasicBlock * mNextStmntTargetBB = nullptr;
    
    /* The llvm basic block for where a 'break' statement jumps to */
    llvm::BasicBlock * mBreakStmntTargetBB = nullptr;
    
    /**
     * Callbacks which are called to generate code after the repeatable statement has
     * been code generated. Certain constructs like 'break' and 'next' need to be 
     * generated AFTER a looping construct is fully fleshed out, because they may need
     * to be able to tie to the end of the loop code.
     */
    std::vector<std::function<void ()>> mDeferredCodegenCallbacks;
    
private:
    WC_DISALLOW_COPY_AND_ASSIGN(RepeatableStmnt)
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
