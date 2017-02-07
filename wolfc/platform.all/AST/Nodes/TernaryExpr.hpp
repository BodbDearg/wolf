#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class LOrExpr;

/*
TernaryExpr:
	LOrExpr
	LOrExpr ? AssignExpr : AssignExpr
*/
class TernaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static TernaryExpr * parse(ParseCtx & parseCtx);
};

/* LOrExpr */
class TernaryExprNoCond final : public TernaryExpr {
public:
    TernaryExprNoCond(LOrExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    LOrExpr & mExpr;
};

/* LOrExpr ? AssignExpr : AssignExpr */
class TernaryExprWithCond final : public TernaryExpr {
public:
    TernaryExprWithCond(LOrExpr & condExpr,
                        AssignExpr & trueExpr,
                        AssignExpr & falseExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    LOrExpr &       mCondExpr;
    AssignExpr &    mTrueExpr;
    AssignExpr &    mFalseExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
