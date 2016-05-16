#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class PrimaryExpr;

/*
UnaryExpr:
	PrimaryExpr
	-PrimaryExpr
	+PrimaryExpr
	(BinaryExpr)
*/
class UnaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * currentToken);
    
    static UnaryExpr * parse(const Token *& currentToken);
};

/* PrimaryExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PrimaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* -PrimaryExpr */
class UnaryExprNegPrimary : public UnaryExpr {
public:
    UnaryExprNegPrimary(PrimaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* +PrimaryExpr */
class UnaryExprPosPrimary : public UnaryExprPrimary {
public:
    UnaryExprPosPrimary(PrimaryExpr & expr);
};

/* (BinaryExpr) */
class UnaryExprParen : public UnaryExpr {
public:
    UnaryExprParen(BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
