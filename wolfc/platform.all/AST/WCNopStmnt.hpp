#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;

/*
NopStmnt:
    nop
*/
class NopStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static NopStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    NopStmnt(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
