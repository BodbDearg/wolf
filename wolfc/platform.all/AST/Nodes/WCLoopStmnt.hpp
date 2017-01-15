#pragma once

#include "WCASTNode.hpp"
#include "WCIRepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
LoopStmnt:
    loop Scope end
*/
class LoopStmnt : public ASTNode, public IRepeatableStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static LoopStmnt * parse(ParseCtx & parseCtx);
    
    LoopStmnt(Scope & bodyScope, const Token & startToken);
    
    virtual const Token & getStartToken() const final override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mBodyScope;
    const Token &   mStartToken;
};

/* loop Scope end */
class LoopStmntNoCond final : public LoopStmnt {
public:
    LoopStmntNoCond(Scope & bodyScope, const Token & startToken, const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::BasicBlock * getNextStmntTargetBlock() override;
    virtual llvm::BasicBlock * getBreakStmntTargetBlock() override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    const Token &       mEndToken;
    llvm::BasicBlock *  mStartBB = nullptr;
    llvm::BasicBlock *  mEndBB = nullptr;
};

/* loop Scope repeat while|until AssignExpr */
class LoopStmntWithCond final : public LoopStmnt {
public:
    LoopStmntWithCond(Scope & bodyScope,
                      const Token & startToken,
                      const Token & condTypeToken,
                      AssignExpr & loopCondExpr);
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::BasicBlock * getNextStmntTargetBlock() override;
    virtual llvm::BasicBlock * getBreakStmntTargetBlock() override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    /**
     * If true this is a loop 'until' statement rather than loop 'while'. In that case
     * The block will execute while the loop condition is false rather than true.
     */
    bool isLoopCondInversed() const;
    
    /**
     * Generate the code for the loop condition expression and return the value of that expression. 
     * Returns nullptr on failure.
     */
    llvm::Value * codegenLoopCondExpr(CodegenCtx & cgCtx) const;
    
    const Token &       mCondTypeToken;
    AssignExpr &        mLoopCondExpr;
    llvm::BasicBlock *  mLoopCondBB = nullptr;
    llvm::BasicBlock *  mEndBB = nullptr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
