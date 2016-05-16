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
    PrintExprStrLit(StrLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* print ( BinaryExpr ) */
class PrintExprBinaryExpr : public PrintExpr {
public:
    PrintExprBinaryExpr(BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
