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

class ArrayLit;
class AssignExpr;
class BoolLit;
class Identifier;
class IntLit;
class NullLit;
class RandExpr;
class ReadnumExpr;
class StrLit;
class TimeExpr;

/*
PrimaryExpr:
	IntLit
	BoolLit
	StrLit
	NullLit
    ArrayLit
    Identifier
    ReadnumExpr
    TimeExpr
    RandExpr
    ( AssignExpr )
*/
class PrimaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PrimaryExpr * parse(ParseCtx & parseCtx);
};

/* IntLit */
class PrimaryExprIntLit final : public PrimaryExpr {
public:
    PrimaryExprIntLit(IntLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    IntLit & mLit;
};

/* BoolLit */
class PrimaryExprBoolLit final : public PrimaryExpr {
public:
    PrimaryExprBoolLit(BoolLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    BoolLit & mLit;
};

/* StrLit */
class PrimaryExprStrLit final : public PrimaryExpr {
public:
    PrimaryExprStrLit(StrLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    StrLit & mLit;
};

/* NullLit */
class PrimaryExprNullLit final : public PrimaryExpr {
public:
    PrimaryExprNullLit(NullLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    NullLit & mLit;
};

/* ArrayLit */
class PrimaryExprArrayLit final : public PrimaryExpr {
public:
    PrimaryExprArrayLit(ArrayLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    ArrayLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier final : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & ident);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const char * name() const;
    
    Identifier & mIdent;
};

/* ReadnumExpr */
class PrimaryExprReadnumExpr final : public PrimaryExpr {
public:
    PrimaryExprReadnumExpr(ReadnumExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    ReadnumExpr & mExpr;
};

/* TimeExpr */
class PrimaryExprTimeExpr final : public PrimaryExpr {
public:
    PrimaryExprTimeExpr(TimeExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    TimeExpr & mExpr;
};

/* RandExpr */
class PrimaryExprRandExpr final : public PrimaryExpr {
public:
    PrimaryExprRandExpr(RandExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    RandExpr & mExpr;
};

/* ( AssignExpr ) */
class PrimaryExprParen final : public PrimaryExpr {
public:
    PrimaryExprParen(const Token & startToken, AssignExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    const Token &   mStartToken;
    AssignExpr &    mExpr;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
