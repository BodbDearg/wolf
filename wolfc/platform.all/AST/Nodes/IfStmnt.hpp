#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class Scope;

/*
IfStmnt:
	if|unless AssignExpr [then] Scope end
    if|unless AssignExpr [then] Scope else [do] Scope end
	if|unless AssignExpr [then] Scope else IfStmnt
*/
class IfStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static IfStmnt * parse(ParseCtx & parseCtx);
    
    IfStmnt(AssignExpr & ifExpr,
            Scope & thenScope,
            const Token & startToken);
    
    virtual const Token & getStartToken() const final override;
    
    /**
     * If true this is an 'unless' statement rather than 'if'. In that case
     * The block will execute if the condition is false. 
     */
    bool isIfExprInversed() const;
    
    AssignExpr &    mIfExpr;
    Scope &         mThenScope;
    const Token &   mStartToken;
};

/* if|unless AssignExpr [then] Scope end */
class IfStmntNoElse final : public IfStmnt {
public:
    IfStmntNoElse(AssignExpr & ifExpr,
                  Scope & thenScope,
                  const Token & startToken,
                  const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    const Token & mEndToken;
};

/* if|unless AssignExpr [then] Scope else [do] Scope end */
class IfStmntElse final : public IfStmnt {
public:
    IfStmntElse(AssignExpr & ifExpr,
                Scope & thenScope,
                Scope & elseScope,
                const Token & startToken,
                const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mElseScope;
    const Token &   mEndToken;
};

/* if|unless AssignExpr [then] Scope else IfStmnt */
class IfStmntElseIf final : public IfStmnt {
public:
    IfStmntElseIf(AssignExpr & ifExpr,
                  Scope & thenScope,
                  IfStmnt & elseIfStmnt,
                  const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    IfStmnt & mElseIfStmnt;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
