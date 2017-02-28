//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class StrLit;

/*
TimeExpr:
    time ( )
*/
class TimeExpr final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static TimeExpr * parse(ParseCtx & parseCtx);
    
    TimeExpr(const Token & startToken, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
