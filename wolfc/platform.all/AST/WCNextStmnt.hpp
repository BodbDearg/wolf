#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class LinearAlloc;

/*
NextStmnt:
	next
*/
class NextStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static NextStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    NextStmnt(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    /* Do the basic forward codegen for the next statement. */
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    /* Shouldn't be called directly. The next statement itself will schedule this. */
    bool deferredCodegen(CodegenCtx & cgCtx);
    
    const Token &       mToken;
    llvm::BasicBlock * 	mBasicBlock = nullptr;
};

WC_END_NAMESPACE
