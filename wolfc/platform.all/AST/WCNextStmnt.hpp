#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class AssignExpr;
class LinearAlloc;

/*
NextStmnt:
	next
	next if|unless AssignExpr
*/
class NextStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static NextStmnt * parse(ParseCtx & parseCtx);
    
    NextStmnt(const Token & nextToken);
    
    virtual const Token & getStartToken() const final override;
    
    /* Shouldn't be called directly. The statement itself will schedule this. */
    bool deferredCodegen(CodegenCtx & cgCtx);
    
    /* The 'next' token */
    const Token & mNextToken;
    
    /* Basic block for the 'next' code */
    llvm::BasicBlock * 	mNextBlock = nullptr;
};

/* next */
class NextStmntNoCond final : public NextStmnt {
public:
    NextStmntNoCond(const Token & nextToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
};

/* next if|unless AssignExpr */
class NextStmntWithCond final : public NextStmnt {
public:
    NextStmntWithCond(const Token & nextToken,
                      const Token & condToken,
                      AssignExpr & condExpr);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Tell if the 'if' condition for the statement is inverted (unless) */
    bool isIfCondInverted() const;

    /* This is either 'if' or 'unless' (inverted condition) */
    const Token & mCondToken;

    /* The condition expression itself */
    AssignExpr & mCondExpr;
    
    /* Basic block for the 'continue' code */
    llvm::BasicBlock * mContinueBlock = nullptr;
};

WC_END_NAMESPACE
