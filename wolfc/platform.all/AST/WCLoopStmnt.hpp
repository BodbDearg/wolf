#pragma once

#include "WCASTNode.hpp"
#include "WCIRepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class LinearAlloc;

/*
LoopStmnt:
    loop Scope end
*/
class LoopStmnt : public ASTNode, public IRepeatableStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static LoopStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    LoopStmnt(Scope & bodyScope, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual llvm::BasicBlock * getStartBlock() override;
    
    virtual llvm::BasicBlock * getEndBlock() override;
    
    Scope &             mBodyScope;
    const Token &       mStartToken;
    llvm::BasicBlock *  mStartBB = nullptr;
    llvm::BasicBlock *  mEndBB = nullptr;
};

/*
loop Scope end
*/
class LoopStmntNoCond : public LoopStmnt {
public:
    LoopStmntNoCond(Scope & bodyScope, const Token & startToken, const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    const Token & mEndToken;
};

WC_END_NAMESPACE
