#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class PrintExpr;
class AssignExpr;
class VarDecl;

/*
Expr:
    PrintExpr
    VarDecl
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
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExpr & mExpr;
};

/* ExprVarDecl */
class ExprVarDecl : public Expr {
public:
    ExprVarDecl(VarDecl & decl);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;

    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    VarDecl & mDecl;
};

/* ExprAssign */
class ExprAssign : public Expr {
public:
    ExprAssign(AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
