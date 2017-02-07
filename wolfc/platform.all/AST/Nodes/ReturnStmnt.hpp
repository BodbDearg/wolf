#pragma once

#include "ASTNode.hpp"
#include "IStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
ReturnStmnt:
	return
	return AssignExpr
    return if|unless AssignExpr
    return AssignExpr if|unless AssignExpr
*/
class ReturnStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static ReturnStmnt * parse(ParseCtx & parseCtx);
    
    ReturnStmnt(const Token & returnToken);
    
    virtual const Token & getStartToken() const final override;
    
    /* The token for 'return' */
    const Token & mReturnToken;
};

/* return */
class ReturnStmntNoCondVoid final : public ReturnStmnt {
public:
    ReturnStmntNoCondVoid(const Token & returnToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
};

/* return AssignExpr */
class ReturnStmntNoCondWithValue final : public ReturnStmnt {
public:
    ReturnStmntNoCondWithValue(const Token & returnToken, AssignExpr & returnExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Expression for the value to return */
    AssignExpr & mReturnExpr;
};

/* This is a base class for return statements with a condition */
class ReturnStmntWithCondBase : public ReturnStmnt {
public:
    ReturnStmntWithCondBase(const Token & returnToken,
                            const Token & condToken,
                            AssignExpr & condExpr);
    
    virtual const Token & getEndToken() const final override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Returns true if the condition for returning is inversed ('unless' instead of 'if') */
    bool isCondExprInversed() const;

    /* Is either 'if' or 'unless' */
    const Token & mCondToken;
    
    /* Expression for the return condition */
    AssignExpr & mCondExpr;
};

/* return if|unless AssignExpr */
class ReturnStmntWithCondVoid final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondVoid(const Token & returnToken,
                            const Token & condToken,
                            AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* return AssignExpr if|unless AssignExpr */
class ReturnStmntWithCondAndValue final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondAndValue(const Token & returnToken,
                                AssignExpr & returnExpr,
                                const Token & condToken,
                                AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;

    /* Expression for the value to return */
    AssignExpr & mReturnExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
