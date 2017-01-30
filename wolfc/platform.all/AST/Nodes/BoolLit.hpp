#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* Represents TokenType::kBoolLit in the AST tree */
class BoolLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static BoolLit * parse(ParseCtx & parseCtx);
    
    BoolLit(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
