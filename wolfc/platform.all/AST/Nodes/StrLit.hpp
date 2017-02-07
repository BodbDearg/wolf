#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* Represents TokenType::kStrLit in the AST tree */
class StrLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static StrLit * parse(ParseCtx & parseCtx);
    
    StrLit(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
