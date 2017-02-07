#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class LAndExpr;

/*
LOrExpr:
	LAndExpr
	LAndExpr or LOrExpr
*/
class LOrExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LOrExpr * parse(ParseCtx & parseCtx);
};

/* LAndExpr */
class LOrExprNoOp final : public LOrExpr {
public:
    LOrExprNoOp(LAndExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    LAndExpr & mExpr;
};

/* LAndExpr or LOrExpr */
class LOrExprOr final : public LOrExpr {
public:
    LOrExprOr(LAndExpr & leftExpr, LOrExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    LAndExpr &  mLeftExpr;
    LOrExpr &   mRightExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
