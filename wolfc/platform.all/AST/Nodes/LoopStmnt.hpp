//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"
#include "IRepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
LoopStmnt:
    loop Scope end
    loop Scope repeat while|until AssignExpr
*/
class LoopStmnt : public ASTNode, public IRepeatableStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static LoopStmnt * parse(ParseCtx & parseCtx);
    
    LoopStmnt(Scope & bodyScope, const Token & startToken);
    
    virtual const Token & getStartToken() const final override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    Scope &         mBodyScope;
    const Token &   mStartToken;
};

/* loop Scope end */
class LoopStmntNoCond final : public LoopStmnt {
public:
    LoopStmntNoCond(Scope & bodyScope, const Token & startToken, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    const Token & mEndToken;
};

/* loop Scope repeat while|until AssignExpr */
class LoopStmntWithCond final : public LoopStmnt {
public:
    LoopStmntWithCond(Scope & bodyScope,
                      const Token & startToken,
                      const Token & condTypeToken,
                      AssignExpr & loopCondExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    /**
     * If true this is a loop 'until' statement rather than loop 'while'. In that case
     * The block will execute while the loop condition is false rather than true.
     */
    bool isLoopCondInversed() const;
    
    const Token &   mCondTypeToken;
    AssignExpr &    mLoopCondExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
