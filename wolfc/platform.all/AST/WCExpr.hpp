#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class PrintExpr;
class AssignExpr;

/*
Expr:
    PrintExpr
    AssignExpr
*/
class Expr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Expr * parse(const Token *& tokenPtr);
};

/* ExprPrint */
class ExprPrint : public Expr {
public:
    ExprPrint(PrintExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExpr & mExpr;
};

/* ExprAssign */
class ExprAssign : public Expr {
public:
    ExprAssign(AssignExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE