#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PostfixExpr;

#warning Rename to prefix expression?
/*
UnaryExpr:
	PostfixExpr
	+ UnaryExpr
	- UnaryExpr
	( AssignExpr )
*/
class UnaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static UnaryExpr * parse(ParseCtx & parseCtx);
};

/* PostfixExpr */
class UnaryExprNoOp : public UnaryExpr {
public:
    UnaryExprNoOp(PostfixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    PostfixExpr & mExpr;
};

/* Base class for '+' or '-' unary expression */
class UnaryExprPlusMinusBase : public UnaryExpr {
public:
    UnaryExprPlusMinusBase(const Token & startToken, UnaryExpr & expr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual const DataType & dataType() const override;
    
    const Token &   mStartToken;
    UnaryExpr &     mExpr;
};

/* + UnaryExpr */
class UnaryExprPlus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprPlus(const Token & startToken, UnaryExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* - UnaryExpr */
class UnaryExprMinus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprMinus(const Token & startToken, UnaryExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ( AssignExpr ) */
class UnaryExprParen final : public UnaryExpr {
public:
    UnaryExprParen(const Token & startToken, AssignExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;

    const Token &   mStartToken;    
    AssignExpr &    mExpr;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
