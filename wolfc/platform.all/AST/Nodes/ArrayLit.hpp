#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace AST {
    class AssignExpr;
}

WC_AST_BEGIN_NAMESPACE

/*
ArrayLit:
    [ [0..N: AssignExpr ,] [AssignExpr] ]
*/
class ArrayLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ArrayLit * parse(ParseCtx & parseCtx);
    
    ArrayLit(const Token & lBrack,
             const std::vector<AssignExpr*> & exprs,
             const Token & rBrack);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    /* Get the sub expressions that constitute the array literal */
    inline const std::vector<const AssignExpr*> & getExprs() const {
        return mExprs;
    }
    
private:
    /* Left bracket token */
    const Token & mLBrack;
    
    /* The assign expression for all array literal sub expressions */
    std::vector<const AssignExpr*> mExprs;
    
    /* Right bracket token */
    const Token & mRBrack;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
