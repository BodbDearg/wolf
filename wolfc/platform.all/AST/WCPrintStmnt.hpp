#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class StrLit;

/*
PrintStmnt
    print ( StringLit )
    print ( BinaryExpr )
*/
class PrintStmnt : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintStmnt * parse(const Token *& tokenPtr);
    
    PrintStmnt(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

/* print ( StringLit ) */
class PrintStmntStrLit : public PrintStmnt {
public:
    PrintStmntStrLit(const Token & startToken, const Token & endToken, StrLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* print ( BinaryExpr ) */
class PrintStmntBinaryExpr : public PrintStmnt {
public:
    PrintStmntBinaryExpr(const Token & startToken, const Token & endToken, BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
