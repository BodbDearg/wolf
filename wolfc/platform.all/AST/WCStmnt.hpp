#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class CodegenCtx;
class PrintStmnt;
class VarDecl;

/*
Stmnt:
    PrintStmnt
    VarDecl
    AssignExpr
*/
class Stmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static Stmnt * parse(const Token *& tokenPtr);
};

/* PrintStmnt */
class StmntPrintStmnt : public Stmnt {
public:
    StmntPrintStmnt(PrintStmnt & stmnt);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    PrintStmnt & mStmnt;
};

/* VarDecl */
class StmntVarDecl : public Stmnt {
public:
    StmntVarDecl(VarDecl & decl);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;

    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    VarDecl & mDecl;
};

/* AssignExpr */
class StmntAssignExpr : public Stmnt {
public:
    StmntAssignExpr(AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
