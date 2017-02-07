#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AddExpr;

/*
CmpExpr:
	AddExpr
	AddExpr == CmpExpr
	AddExpr is CmpExpr
	AddExpr != CmpExpr
	AddExpr is not CmpExpr
	AddExpr < CmpExpr
	AddExpr <= CmpExpr
	AddExpr > CmpExpr
	AddExpr >= CmpExpr
*/
class CmpExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static CmpExpr * parse(ParseCtx & parseCtx);
};

/* AddExpr */
class CmpExprNoOp final : public CmpExpr {
public:
    CmpExprNoOp(AddExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    AddExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class CmpExprTwoOps : public CmpExpr {
public:
    CmpExprTwoOps(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual const DataType & dataType() const final override;
    
    AddExpr & mLeftExpr;
    CmpExpr & mRightExpr;
};

/**
 * AddExpr == CmpExpr
 * AddExpr is CmpExpr
 */
class CmpExprEQ final : public CmpExprTwoOps {
public:
    CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/** 
 * AddExpr != CmpExpr
 * AddExpr is not CmpExpr
 */
class CmpExprNE final : public CmpExprTwoOps {
public:
    CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* AddExpr < CmpExpr */
class CmpExprLT final : public CmpExprTwoOps {
public:
    CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* AddExpr <= CmpExpr */
class CmpExprLE final : public CmpExprTwoOps {
public:
    CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* AddExpr > CmpExpr */
class CmpExprGT final : public CmpExprTwoOps {
public:
    CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* AddExpr >= CmpExpr */
class CmpExprGE final : public CmpExprTwoOps {
public:
    CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
