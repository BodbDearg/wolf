#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
BreakStmnt:
	break
	break if|unless AssignExpr
*/
class BreakStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static BreakStmnt * parse(ParseCtx & parseCtx);
    
    BreakStmnt(const Token & breakToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* The 'break' token */
    const Token & mBreakToken;
    
#warning FIXME - Codegen
#if 0
    /* Basic block for the 'break' code */
    llvm::BasicBlock * mBreakBlock = nullptr;
#endif
};

/* break */
class BreakStmntNoCond final : public BreakStmnt {
public:
    BreakStmntNoCond(const Token & breakToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
};

/* break if|unless AssignExpr */
class BreakStmntWithCond final : public BreakStmnt {
public:
    BreakStmntWithCond(const Token & breakToken,
                       const Token & condToken,
                       AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    /* Tell if the 'if' condition for the statement is inverted (unless) */
    bool isIfCondInverted() const;

    /* This is either 'if' or 'unless' (inverted condition) */
    const Token & mCondToken;

    /* The condition expression itself */
    AssignExpr & mCondExpr;
    
#warning FIXME - Codegen
#if 0
    /* Basic block for the 'continue' code */
    llvm::BasicBlock * mContinueBlock = nullptr;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
