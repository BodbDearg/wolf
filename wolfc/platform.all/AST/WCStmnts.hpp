#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class Stmnt;

/*
Stmnts:
    Stmnt
    Stmnt Stmnts
*/
class Stmnts : public ASTNode, public IStmnt {
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
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    Stmnt & mStmnt;
};

/* Stmnt Stmnts */
class StmntsMulti : public Stmnts {
public:
    StmntsMulti(Stmnt & leftStmnt, Stmnts & rightStmnts);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    Stmnt & mLeftStmnt;
    Stmnts & mRightStmnts;
};

WC_END_NAMESPACE
