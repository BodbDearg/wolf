#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PostfixExpr;

#warning Rename to prefix expression?
/*
UnaryExpr:
	PostfixExpr
	+ PostfixExpr
	- PostfixExpr
	( AssignExpr )
*/
class UnaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static UnaryExpr * parse(ParseCtx & parseCtx);
};

#warning This needs to be renamed
/* PostfixExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PostfixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    PostfixExpr & mExpr;
};

/* Base class for '+' or '-' unary expression */
class UnaryExprPlusMinusBase : public UnaryExprPrimary {
public:
    UnaryExprPlusMinusBase(PostfixExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() const final override;
    
    const Token & mStartToken;
};

/* + PostfixExpr */
class UnaryExprPlus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprPlus(PostfixExpr & expr, const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* - PostfixExpr */
class UnaryExprMinus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprMinus(PostfixExpr & expr, const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ( AssignExpr ) */
class UnaryExprParen final : public UnaryExpr {
public:
    UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;

    AssignExpr &    mExpr;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
