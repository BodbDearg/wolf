#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class StrLit;

/*
PrintExpr
    print ( StringLit )
    print ( BinaryExpr )
*/
class PrintExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintExpr * parse(ASTNode & parent, const Token *& tokenPtr);
    
    PrintExpr(ASTNode & parent);
};

/* print ( StringLit ) */
class PrintExprStrLit : public PrintExpr {
public:
    static PrintExprStrLit * parse(ASTNode & parent, const Token *& tokenPtr);
    
    PrintExprStrLit(ASTNode & parent, StrLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* print ( BinaryExpr ) */
class PrintExprBinaryExpr : public PrintExpr {
public:
    static PrintExprBinaryExpr * parse(ASTNode & parent, const Token *& tokenPtr);
    
    PrintExprBinaryExpr(ASTNode & parent, BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
