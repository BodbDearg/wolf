#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class CmpExpr;

/*
NotExpr:
	CmpExpr
	not NotExpr
	~ NotExpr
*/
class NotExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static NotExpr * parse(ParseCtx & parseCtx);
};

/* CmpExpr */
class NotExprNoOp final : public NotExpr {
public:
    NotExprNoOp(CmpExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    CmpExpr & mExpr;
};

/* not NotExpr */
class NotExprLNot final : public NotExpr {
public:
    NotExprLNot(NotExpr & expr, const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    NotExpr &       mExpr;
    const Token &   mStartToken;
};

/* ~ NotExpr */
class NotExprBNot final : public NotExpr {
public:
    NotExprBNot(NotExpr & expr, const Token & startToken);

    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;

    virtual const DataType & dataType() const override;

    NotExpr &       mExpr;
    const Token &   mStartToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
