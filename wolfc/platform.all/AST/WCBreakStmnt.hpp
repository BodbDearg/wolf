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
BreakStmnt:
	break
	break if|unless AssignExpr
*/
class BreakStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static BreakStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    BreakStmnt(const Token & breakToken);
    
    virtual const Token & getStartToken() const override;
    
    /* Shouldn't be called directly. The statement itself will schedule this. */
    bool deferredCodegen(CodegenCtx & cgCtx);
    
    /* The 'break' token */
    const Token & mBreakToken;
    
    /* Basic block for the 'break' code */
    llvm::BasicBlock * mBreakBlock = nullptr;
};

/* break */
class BreakStmntNoCond : public BreakStmnt {
public:
    BreakStmntNoCond(const Token & breakToken);
    
    virtual const Token & getEndToken() const override;
    
    /* Do the basic forward codegen for the statement. */
    virtual bool codegen(CodegenCtx & cgCtx) override;
};

/* break if|unless AssignExpr */
class BreakStmntWithCond : public BreakStmnt {
public:
    BreakStmntWithCond(const Token & breakToken,
                       const Token & condToken,
                       AssignExpr & condExpr);
    
    virtual const Token & getEndToken() const override;
    
    /* Do the basic forward codegen for the statement. */
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
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
