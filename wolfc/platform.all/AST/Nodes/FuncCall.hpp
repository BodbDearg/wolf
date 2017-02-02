#pragma once

#include "ASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
FuncCall:
	( [0..N: AssignExpr ,][AssignExpr] )
*/
class FuncCall final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static FuncCall * parse(ParseCtx & parseCtx);
    
    FuncCall(const Token & startToken,
             const std::vector<AssignExpr*> & args,
             const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    inline const std::vector<const AssignExpr*> & getArgs() const {
        return mArgs;
    }
    
private:
    const Token & mStartToken;
    const Token & mEndToken;
    
    /* The assign expression for all the arguments */
    std::vector<const AssignExpr*> mArgs;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
