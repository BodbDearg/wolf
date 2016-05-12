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
    
    static PrintExpr * parse(const Token *& tokenPtr);
};

/* print ( StringLit ) */
class PrintExprStrLit : public PrintExpr {
public:
    static PrintExprStrLit * parse(const Token *& tokenPtr);
    
    PrintExprStrLit(StrLit & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    StrLit & mLit;
};

/* print ( BinaryExpr ) */
class PrintExprBinaryExpr : public PrintExpr {
public:
    static PrintExprBinaryExpr * parse(const Token *& tokenPtr);
    
    PrintExprBinaryExpr(BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
