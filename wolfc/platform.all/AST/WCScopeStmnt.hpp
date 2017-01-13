#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class Scope;

/*
ScopeStmnt:
	scope Scope end
*/
class ScopeStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static ScopeStmnt * parse(ParseCtx & parseCtx);
    
    ScopeStmnt(const Token & startToken, Scope & bodyScope, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mBodyScope;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
