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

class PostfixExpr;

/*
PrefixExpr:
	PostfixExpr
	+ PrefixExpr
	- PrefixExpr
	& PrefixExpr
	$ PrefixExpr
	^ PrefixExpr
*/
class PrefixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PrefixExpr * parse(ParseCtx & parseCtx);
};

/* PostfixExpr */
class PrefixExprNoOp : public PrefixExpr {
public:
    PrefixExprNoOp(PostfixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    PostfixExpr & mExpr;
};

/* Base class for '+' or '-' etc. unary expression */
class PrefixExprWithUnaryOp : public PrefixExpr {
public:
    PrefixExprWithUnaryOp(const Token & startToken, PrefixExpr & expr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    const Token &   mStartToken;
    PrefixExpr &    mExpr;
};

/* + PrefixExpr */
class PrefixExprPlus final : public PrefixExprWithUnaryOp {
public:
    PrefixExprPlus(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* - PrefixExpr */
class PrefixExprMinus final : public PrefixExprWithUnaryOp {
public:
    PrefixExprMinus(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* & PrefixExpr */
class PrefixExprAddrOf final : public PrefixExprWithUnaryOp {
public:
    PrefixExprAddrOf(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* $ PrefixExpr */
class PrefixExprPtrDeref final : public PrefixExprWithUnaryOp {
public:
    PrefixExprPtrDeref(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ^ PrefixExpr */
class PrefixExprPtrDenull final : public PrefixExprWithUnaryOp {
public:
    PrefixExprPtrDenull(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
