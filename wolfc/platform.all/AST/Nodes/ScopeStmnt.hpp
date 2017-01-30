#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

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
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mBodyScope;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
