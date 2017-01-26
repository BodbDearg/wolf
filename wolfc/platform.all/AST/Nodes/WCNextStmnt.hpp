#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
NextStmnt:
	next
	next if|unless AssignExpr
*/
class NextStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static NextStmnt * parse(ParseCtx & parseCtx);
    
    NextStmnt(const Token & nextToken);
    
    virtual const Token & getStartToken() const final override;
    
    /* The 'next' token */
    const Token & mNextToken;
    
#warning FIXME - Codegen
#if 0
    /* Basic block for the 'next' code */
    llvm::BasicBlock * 	mNextBlock = nullptr;
#endif
};

/* next */
class NextStmntNoCond final : public NextStmnt {
public:
    NextStmntNoCond(const Token & nextToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
};

/* next if|unless AssignExpr */
class NextStmntWithCond final : public NextStmnt {
public:
    NextStmntWithCond(const Token & nextToken,
                      const Token & condToken,
                      AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
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
