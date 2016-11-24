#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

class LinearAlloc;
class TernaryExpr;
struct CodegenCtx;

/*
NextStmnt:
	next
	next if|unless TernaryExpr
*/
class NextStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static NextStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
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
    
    /* Do the basic forward codegen for the statement. */
    virtual bool codegen(CodegenCtx & cgCtx) override;
};

/* next if|unless TernaryExpr */
class NextStmntWithCond final : public NextStmnt {
public:
    NextStmntWithCond(const Token & nextToken,
                      const Token & condToken,
                      TernaryExpr & condExpr);
    
    virtual const Token & getEndToken() const override;
    
    /* Do the basic forward codegen for the statement. */
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    /* Tell if the 'if' condition for the statement is inverted (unless) */
    bool isIfCondInverted() const;

    /* This is either 'if' or 'unless' (inverted condition) */
    const Token & mCondToken;

    /* The condition expression itself */
    TernaryExpr & mCondExpr;
    
    /* Basic block for the 'continue' code */
    llvm::BasicBlock * mContinueBlock = nullptr;
};

WC_END_NAMESPACE
