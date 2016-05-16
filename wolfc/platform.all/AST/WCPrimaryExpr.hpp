#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class IntLit;
class Identifier;

/*
PrimaryExpr:
    IntLit
    Identifier
*/
class PrimaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * currentToken);
    
    static PrimaryExpr * parse(const Token *& currentToken);
};

/* IntLit */
class PrimaryExprIntLit : public PrimaryExpr {
public:
    PrimaryExprIntLit(IntLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & identifier);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Identifier & mIdentifier;
};

WC_END_NAMESPACE
