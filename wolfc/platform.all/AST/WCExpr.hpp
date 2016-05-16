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
    
    static Expr * parse(ASTNode & parent, const Token *& tokenPtr);
    
    Expr(ASTNode & parent);
};

/* ExprPrint */
class ExprPrint : public Expr {
public:
    ExprPrint(ASTNode & parent, PrintExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExpr & mExpr;
};

/* ExprAssign */
class ExprAssign : public Expr {
public:
    ExprAssign(ASTNode & parent, AssignExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
