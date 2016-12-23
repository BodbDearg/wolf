#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;

/*
NoOpStmnt:
    noop
*/
class NoOpStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static NoOpStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    NoOpStmnt(const Token & token);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
