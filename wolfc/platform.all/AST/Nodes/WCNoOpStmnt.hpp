#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/*
NoOpStmnt:
    noop
*/
class NoOpStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static NoOpStmnt * parse(ParseCtx & parseCtx);
    
    NoOpStmnt(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
