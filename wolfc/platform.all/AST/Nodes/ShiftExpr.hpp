#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class UnaryExpr;

/*
ShiftExpr:
    UnaryExpr
	UnaryExpr << ShiftExpr
	UnaryExpr >> ShiftExpr
	UnaryExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(ParseCtx & parseCtx);
};

/* UnaryExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(UnaryExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    UnaryExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    UnaryExpr & mLeftExpr;
    ShiftExpr & mRightExpr;
};

/* UnaryExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* UnaryExpr >> ShiftExpr */
class ShiftExprARShift final : public ShiftExprTwoOps {
public:
    ShiftExprARShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* UnaryExpr >>> ShiftExpr */
class ShiftExprLRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
