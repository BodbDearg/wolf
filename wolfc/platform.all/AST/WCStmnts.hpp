#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class Stmnt;

/*
Stmnts:
    Stmnt
    Stmnt Stmnts
*/
class Stmnts : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Stmnts * parse(const Token *& tokenPtr);
};

/* Stmnt */
class StmntsSingle : public Stmnts {
public:
    StmntsSingle(Stmnt & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Stmnt & mStmnt;
};

/* Stmnt Stmnts */
class StmntsMulti : public Stmnts {
public:
    StmntsMulti(Stmnt & leftStmnt, Stmnts & rightStmnts);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Stmnt & mLeftStmnt;
    Stmnts & mRightStmnts;
};

WC_END_NAMESPACE
