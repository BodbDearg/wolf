#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class LinearAlloc;

/*
AssertStmnt:
    assert ( AssignExpr )
*/
class AssertStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static AssertStmnt * parse(ParseCtx & parseCtx);
    
    AssertStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    const Token &   mStartToken;
    AssignExpr &    mExpr;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
