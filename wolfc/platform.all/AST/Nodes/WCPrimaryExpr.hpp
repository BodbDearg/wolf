#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class ArrayLit;
class BoolLit;
class Identifier;
class IntLit;
class RandExpr;
class ReadnumExpr;
class StrLit;
class TimeExpr;

/*
PrimaryExpr:
	IntLit
	BoolLit
	StrLit
    ArrayLit
    Identifier
    ReadnumExpr
    TimeExpr
    RandExpr
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
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    IntLit & mLit;
};

/* BoolLit */
class PrimaryExprBoolLit final : public PrimaryExpr {
public:
    PrimaryExprBoolLit(BoolLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    BoolLit & mLit;
};

/* StrLit */
class PrimaryExprStrLit final : public PrimaryExpr {
public:
    PrimaryExprStrLit(StrLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    StrLit & mLit;
};

/* ArrayLit */
class PrimaryExprArrayLit final : public PrimaryExpr {
public:
    PrimaryExprArrayLit(ArrayLit & lit);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    ArrayLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier final : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & ident);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    const char * name() const;
    
    Identifier & mIdent;
};

/* ReadnumExpr */
class PrimaryExprReadnum final : public PrimaryExpr {
public:
    PrimaryExprReadnum(ReadnumExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    ReadnumExpr & mExpr;
};

/* TimeExpr */
class PrimaryExprTime final : public PrimaryExpr {
public:
    PrimaryExprTime(TimeExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    TimeExpr & mExpr;
};

/* RandExpr */
class PrimaryExprRandExpr final : public PrimaryExpr {
public:
    PrimaryExprRandExpr(RandExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    RandExpr & mExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
