#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class Expr;

/*
Exprs:
    Expr
    Expr Exprs
*/
class Exprs : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Exprs * parse(const Token *& tokenPtr);
};

/* Expr */
class ExprsSingle : public Exprs {
public:
    ExprsSingle(Expr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Expr & mExpr;
};

/* Expr Exprs */
class ExprsMulti : public Exprs {
public:
    ExprsMulti(Expr & leftExpr, Exprs & rightExprs);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Expr & mLeftExpr;
    Exprs & mRightExprs;
};

WC_END_NAMESPACE
