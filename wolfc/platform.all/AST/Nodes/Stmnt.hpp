#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssertStmnt;
class AssignExpr;
class BreakStmnt;
class IfStmnt;
class LoopStmnt;
class NextStmnt;
class NoOpStmnt;
class PrintStmnt;
class ReturnStmnt;
class ScopeStmnt;
class VarDecl;
class WhileStmnt;

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
    static Stmnt * parse(ParseCtx & parseCtx);
};

/* NoOpStmnt */
class StmntNoOpStmnt final : public Stmnt {
public:
    StmntNoOpStmnt(NoOpStmnt & stmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    NoOpStmnt & mStmnt;
};

/* PrintStmnt */
class StmntPrintStmnt final : public Stmnt {
public:
    StmntPrintStmnt(PrintStmnt & stmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    PrintStmnt & mStmnt;
};

/* AssertStmnt */
class StmntAssertStmnt final : public Stmnt {
public:
    StmntAssertStmnt(AssertStmnt & stmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    AssertStmnt & mStmnt;
};

/* IfStmnt */
class StmntIfStmnt final : public Stmnt {
public:
    StmntIfStmnt(IfStmnt & ifStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    IfStmnt & mIfStmnt;
};

/* WhileStmnt */
class StmntWhileStmnt final : public Stmnt {
public:
    StmntWhileStmnt(WhileStmnt & ifStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    WhileStmnt & mWhileStmnt;
};

/* LoopStmnt */
class StmntLoopStmnt final : public Stmnt {
public:
    StmntLoopStmnt(LoopStmnt & loopStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    LoopStmnt & mLoopStmnt;
};

/* ScopeStmnt */
class StmntScopeStmnt final : public Stmnt {
public:
    StmntScopeStmnt(ScopeStmnt & scopeStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    ScopeStmnt & mScopeStmnt;
};

/* BreakStmnt */
class StmntBreakStmnt final : public Stmnt {
public:
    StmntBreakStmnt(BreakStmnt & breakStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    BreakStmnt & mBreakStmnt;
};

/* NextStmnt */
class StmntNextStmnt final : public Stmnt {
public:
    StmntNextStmnt(NextStmnt & nextStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    NextStmnt & mNextStmnt;
};

/* ReturnStmnt */
class StmntReturnStmnt final : public Stmnt {
public:
    StmntReturnStmnt(ReturnStmnt & returnStmnt);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    ReturnStmnt & mReturnStmnt;
};

/* VarDecl */
class StmntVarDecl final : public Stmnt {
public:
    StmntVarDecl(VarDecl & decl);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    VarDecl & mDecl;
};

/* AssignExpr */
class StmntAssignExpr final : public Stmnt {
public:
    StmntAssignExpr(AssignExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    AssignExpr & mExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
