//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"
#include "CStrComparator.hpp"
#include "IStmnt.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class Stmnt;
class VarDecl;

// TODO: not a statement, don't inherit from IStmnt

/*
Scope:
    [0..N: Stmnt|,]
*/
class Scope final : public ASTNode, public IStmnt {
public:
    static Scope * parse(ParseCtx & parseCtx);
    
    Scope(const Token & startToken, std::vector<Stmnt*> && stmnts);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Get all the statements in the scope */
    inline const std::vector<Stmnt*> & getStmnts() const {
        return mStmnts;
    }
    
private:
    /* All the statements in the scope. */
    std::vector<Stmnt*> mStmnts;
    
    /* The start token that the scope started off with */
    const Token & mStartToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
