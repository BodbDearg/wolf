#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PostfixExpr;

/*
PrefixExpr:
	PostfixExpr
	+ PrefixExpr
	- PrefixExpr
	( AssignExpr )
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

/* Base class for '+' or '-' unary expression */
class PrefixExprPlusMinusBase : public PrefixExpr {
public:
    PrefixExprPlusMinusBase(const Token & startToken, PrefixExpr & expr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    const Token &   mStartToken;
    PrefixExpr &     mExpr;
};

/* + PrefixExpr */
class PrefixExprPlus final : public PrefixExprPlusMinusBase {
public:
    PrefixExprPlus(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* - PrefixExpr */
class PrefixExprMinus final : public PrefixExprPlusMinusBase {
public:
    PrefixExprMinus(const Token & startToken, PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ( AssignExpr ) */
class PrefixExprParen final : public PrefixExpr {
public:
    PrefixExprParen(const Token & startToken, AssignExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    const Token &   mStartToken;    
    AssignExpr &    mExpr;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE