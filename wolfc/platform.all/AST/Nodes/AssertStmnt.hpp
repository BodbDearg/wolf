//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
AssertStmnt:
    assert ( AssignExpr )
*/
class AssertStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static AssertStmnt * parse(ParseCtx & parseCtx);
    
    AssertStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    const Token &   mStartToken;
    AssignExpr &    mExpr;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
