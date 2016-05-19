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
    
    PrintExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

/* print ( StringLit ) */
class PrintExprStrLit : public PrintExpr {
public:
    PrintExprStrLit(const Token & startToken, const Token & endToken, StrLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* print ( BinaryExpr ) */
class PrintExprBinaryExpr : public PrintExpr {
public:
    PrintExprBinaryExpr(const Token & startToken, const Token & endToken, BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
