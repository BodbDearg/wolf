#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PrimaryExpr;
class Type;

#warning TODO: This can go under 'UnaryExpr' (or 'PrefixExpr' as it will be renamed).
/*
CastExpr:
	PrimaryExpr
	cast ( AssignExpr to Type )
*/
class CastExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static CastExpr * parse(ParseCtx & parseCtx);
};

/* PrimaryExpr */
class CastExprNoCast final : public CastExpr {
public:
    CastExprNoCast(PrimaryExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    PrimaryExpr & mExpr;
};

/* cast ( AssignExpr to Type ) */
class CastExprCast final : public CastExpr {
public:
    CastExprCast(const Token & startToken,
                 AssignExpr & expr,
                 Type & type,
                 const Token & endToken);

    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    const Token &   mStartToken;
    AssignExpr &    mExpr;
    Type &          mType;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
