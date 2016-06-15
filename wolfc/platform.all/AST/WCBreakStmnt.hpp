#pragma once

#pragma once

#include "WCASTNode.hpp"
#include "WCIDeferredCodegenStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

class LinearAlloc;

/*
BreakStmnt:
	break
*/
class BreakStmnt : public ASTNode, public IDeferredCodegenStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static BreakStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    BreakStmnt(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    virtual bool deferredCodegenStmnt(CodegenCtx & cgCtx) override;
    
    const Token &       mToken;
    llvm::BasicBlock * 	mBasicBlock = nullptr;
};

WC_END_NAMESPACE
