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

class MulExpr;

/*
AddExpr:
    MulExpr
    MulExpr + AddExpr
    MulExpr - AddExpr
    MulExpr | AddExpr
    MulExpr ^ AddExpr
*/
class AddExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AddExpr * parse(ParseCtx & parseCtx);
};

/* MulExpr */
class AddExprNoOp final : public AddExpr {
public:
    AddExprNoOp(MulExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    MulExpr & mExpr;
};

/* Base class for an AddExpr with two operands */
class AddExprTwoOps : public AddExpr {
public:
    AddExprTwoOps(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    MulExpr & mLeftExpr;
    AddExpr & mRightExpr;
};

/* MulExpr + AddExpr */
class AddExprAdd final : public AddExprTwoOps {
public:
    AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* MulExpr - AddExpr */
class AddExprSub final : public AddExprTwoOps {
public:
    AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* MulExpr | AddExpr */
class AddExprBOr final : public AddExprTwoOps {
public:
    AddExprBOr(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* MulExpr ^ AddExpr */
class AddExprBXor final : public AddExprTwoOps {
public:
    AddExprBXor(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
