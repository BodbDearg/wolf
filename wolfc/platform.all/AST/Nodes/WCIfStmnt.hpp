#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class Scope;

/*
IfStmnt:
	if|unless AssignExpr [then] Scope end
	if|unless AssignExpr [then] Scope or IfStmnt
	if|unless AssignExpr [then] Scope else Scope end
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

/* if|unless AssignExpr [then] Scope or IfStmnt */
class IfStmntElseIf final : public IfStmnt {
public:
    IfStmntElseIf(AssignExpr & ifExpr,
                  Scope & thenScope,
                  IfStmnt & elseIfStmnt,
                  const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    IfStmnt & mElseIfStmnt;
};

/* if|unless AssignExpr [then] Scope else Scope end */
class IfStmntElse final : public IfStmnt {
public:
    IfStmntElse(AssignExpr & ifExpr,
                Scope & thenScope,
                Scope & elseScope,
                const Token & startToken,
                const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mElseScope;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
