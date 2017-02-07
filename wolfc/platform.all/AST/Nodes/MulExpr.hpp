#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class ShiftExpr;

/*
MulExpr:
	ShiftExpr
	ShiftExpr * MulExpr
	ShiftExpr / MulExpr
	ShiftExpr % MulExpr
*/
class MulExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static MulExpr * parse(ParseCtx & parseCtx);
};

/* ShiftExpr */
class MulExprNoOp final : public MulExpr {
public:
    MulExprNoOp(ShiftExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    ShiftExpr & mExpr;
};

/* Base class for an MulExpr with two operands */
class MulExprTwoOps : public MulExpr {
public:
    MulExprTwoOps(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual const DataType & dataType() const final override;
    
    ShiftExpr & mLeftExpr;
    MulExpr &   mRightExpr;
};

/* ShiftExpr * MulExpr */
class MulExprMul final : public MulExprTwoOps {
public:
    MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ShiftExpr / MulExpr */
class MulExprDiv final : public MulExprTwoOps {
public:
    MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ShiftExpr % MulExpr */
class MulExprMod final : public MulExprTwoOps {
public:
    MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* ShiftExpr & MulExpr */
class MulExprBAnd final : public MulExprTwoOps {
public:
    MulExprBAnd(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
