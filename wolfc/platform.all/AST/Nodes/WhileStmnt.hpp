#pragma once

#include "ASTNode.hpp"
#include "IRepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
WhileStmnt:
	while|until AssignExpr [do] Scope end
*/
class WhileStmnt final : public ASTNode, public IRepeatableStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static WhileStmnt * parse(ParseCtx & parseCtx);
    
    WhileStmnt(AssignExpr & whileExpr,
               Scope & bodyScope,
               const Token & startToken,
               const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /**
     * If true this is an 'until' statement rather than 'while'. In that case
     * The block will execute while the loop condition is false rather than true.
     */
    bool isWhileExprInversed() const;
    
    AssignExpr &    mWhileExpr;
    Scope &         mBodyScope;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
