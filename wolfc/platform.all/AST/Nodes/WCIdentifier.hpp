#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static Identifier * parse(ParseCtx & parseCtx);
    
    Identifier(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    const char * name() const;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
