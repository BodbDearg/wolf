#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class CastExpr;

/*
PostfixExpr:
	CastExpr
	CastExpr ++
	CastExpr --
	PostfixExpr ( [0..N: AssignExpr ,][AssignExpr] )
	PostfixExpr [ AssignExpr ]
*/
class PostfixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PostfixExpr * parse(ParseCtx & parseCtx);
};

/* CastExpr */
class PostfixExprNoOp final : public PostfixExpr {
public:
    PostfixExprNoOp(CastExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    CastExpr & mExpr;
};

/* Base class for increment/decrement postfix expressions */
class PostfixExprIncDecBase : public PostfixExpr {
public:
    PostfixExprIncDecBase(CastExpr & expr, const Token & endToken);

    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    CastExpr &      mExpr;
    const Token &   mEndToken;
};

/* CastExpr ++ */
class PostfixExprInc final : public PostfixExprIncDecBase {
public:
    PostfixExprInc(CastExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* CastExpr -- */
class PostfixExprDec final : public PostfixExprIncDecBase {
public:
    PostfixExprDec(CastExpr & expr, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* PostfixExpr ( [0..N: AssignExpr ,][AssignExpr] ) */
class PostfixExprFuncCall final : public PostfixExpr {
public:
    PostfixExprFuncCall(PostfixExpr & operandExpr,
                        const Token & callOpeningParen,
                        std::vector<AssignExpr*> & argExprs,
                        const Token & callClosingParen);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    /* The thing which the function is being called on */
    PostfixExpr & mOperandExpr;
    
    /* The opening parenthesis '(' of the function call */
    const Token & mCallOpeningParen;
    
    /* The assign expression for all the arguments */
    std::vector<const AssignExpr*> mArgExprs;
    
    /* The closing parenthesis ')' of the function call */
    const Token & mCallClosingParen;
};

/* PostfixExpr [ AssignExpr ] */
class PostfixExprArrayLookup final : public PostfixExpr {
public:
    PostfixExprArrayLookup(PostfixExpr & arrayExpr,
                           AssignExpr & indexExpr,
                           const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    PostfixExpr &   mArrayExpr;
    AssignExpr &    mIndexExpr;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
