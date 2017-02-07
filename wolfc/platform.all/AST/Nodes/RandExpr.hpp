#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class StrLit;

/*
RandExpr:
    rand ( )
	srand ( AssignExpr )
*/
class RandExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static RandExpr * parse(ParseCtx & parseCtx);
    
    RandExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

/* rand ( ) */
class RandExprRand final : public RandExpr {
public:
    RandExprRand(const Token & startToken, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* srand ( AssignExpr ) */
class RandExprSRand final : public RandExpr {
public:
    RandExprSRand(const Token & startToken,
                  AssignExpr & seedExpr,
                  const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    AssignExpr & mSeedExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
