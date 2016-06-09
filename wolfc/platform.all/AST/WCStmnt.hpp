#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class CodegenCtx;
class IfStmnt;
class NopStmnt;
class PrintStmnt;
class VarDecl;

/*
Stmnt:
	NoOpStmnt
    PrintStmnt
    VarDecl
    IfStmnt
    AssignExpr
*/
class Stmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static Stmnt * parse(const Token *& tokenPtr);
};

/* NopStmnt */
class StmntNop : public Stmnt {
public:
    StmntNop(NopStmnt & stmnt);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    NopStmnt & mStmnt;
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

/* IfStmnt */
class StmntIfStmnt : public Stmnt {
public:
    StmntIfStmnt(IfStmnt & ifStmnt);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;

    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    IfStmnt & mIfStmnt;
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
