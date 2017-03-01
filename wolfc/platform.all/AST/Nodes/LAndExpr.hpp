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

class NotExpr;

/*
LAndExpr:
    NotExpr
    NotExpr and LAndExpr
*/
class LAndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LAndExpr * parse(ParseCtx & parseCtx);
};

/* NotExpr */
class LAndExprNoOp final : public LAndExpr {
public:
    LAndExprNoOp(NotExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    NotExpr & mExpr;
};

/* NotExpr and AndExpr */
class LAndExprAnd final : public LAndExpr {
public:
    LAndExprAnd(NotExpr & leftExpr, LAndExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    NotExpr &   mLeftExpr;
    LAndExpr &  mRightExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
