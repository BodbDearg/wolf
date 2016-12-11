#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssertStmnt;
class AssignExpr;
class BreakStmnt;
class IfStmnt;
class LinearAlloc;
class LoopStmnt;
class NextStmnt;
class NopStmnt;
class PrintStmnt;
class ReturnStmnt;
class ScopeStmnt;
class VarDecl;
class WhileStmnt;
struct CodegenCtx;

/*
Stmnt:
	NoOpStmnt
    PrintStmnt
    AssertStmnt
    VarDecl
    IfStmnt
    WhileStmnt
    LoopStmnt
    ScopeStmnt
	BreakStmnt
	NextStmnt
	ReturnStmnt
	AssignExpr
*/
class Stmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static Stmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* NopStmnt */
class StmntNopStmnt final : public Stmnt {
public:
    StmntNopStmnt(NopStmnt & stmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    NopStmnt & mStmnt;
};

/* PrintStmnt */
class StmntPrintStmnt final : public Stmnt {
public:
    StmntPrintStmnt(PrintStmnt & stmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    PrintStmnt & mStmnt;
};

/* AssertStmnt */
class StmntAssertStmnt final : public Stmnt {
public:
    StmntAssertStmnt(AssertStmnt & stmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    AssertStmnt & mStmnt;
};

/* IfStmnt */
class StmntIfStmnt final : public Stmnt {
public:
    StmntIfStmnt(IfStmnt & ifStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    IfStmnt & mIfStmnt;
};

/* WhileStmnt */
class StmntWhileStmnt final : public Stmnt {
public:
    StmntWhileStmnt(WhileStmnt & ifStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    WhileStmnt & mWhileStmnt;
};

/* LoopStmnt */
class StmntLoopStmnt final : public Stmnt {
public:
    StmntLoopStmnt(LoopStmnt & loopStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    LoopStmnt & mLoopStmnt;
};

/* ScopeStmnt */
class StmntScopeStmnt final : public Stmnt {
public:
    StmntScopeStmnt(ScopeStmnt & scopeStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    ScopeStmnt & mScopeStmnt;
};

/* BreakStmnt */
class StmntBreakStmnt final : public Stmnt {
public:
    StmntBreakStmnt(BreakStmnt & breakStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    BreakStmnt & mBreakStmnt;
};

/* NextStmnt */
class StmntNextStmnt final : public Stmnt {
public:
    StmntNextStmnt(NextStmnt & nextStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    NextStmnt & mNextStmnt;
};

/* ReturnStmnt */
class StmntReturnStmnt final : public Stmnt {
public:
    StmntReturnStmnt(ReturnStmnt & returnStmnt);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    ReturnStmnt & mReturnStmnt;
};

/* VarDecl */
class StmntVarDecl final : public Stmnt {
public:
    StmntVarDecl(VarDecl & decl);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    VarDecl & mDecl;
};

/* AssignExpr */
class StmntAssignExpr final : public Stmnt {
public:
    StmntAssignExpr(AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
