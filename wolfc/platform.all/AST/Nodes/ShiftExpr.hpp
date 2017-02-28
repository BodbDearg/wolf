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

class PrefixExpr;

/*
ShiftExpr:
    PrefixExpr
	PrefixExpr << ShiftExpr
	PrefixExpr >> ShiftExpr
	PrefixExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(ParseCtx & parseCtx);
};

/* PrefixExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(PrefixExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    PrefixExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(PrefixExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    PrefixExpr & mLeftExpr;
    ShiftExpr & mRightExpr;
};

/* PrefixExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* PrefixExpr >> ShiftExpr */
class ShiftExprARShift final : public ShiftExprTwoOps {
public:
    ShiftExprARShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* PrefixExpr >>> ShiftExpr */
class ShiftExprLRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLRShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
