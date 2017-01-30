#pragma once

#include "ASTNode.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

/*
PrimitiveType:
	void|int|int64|bool|string
*/
class PrimitiveType final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static PrimitiveType * parse(ParseCtx & parseCtx);
    
    PrimitiveType(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const DataType & dataType() const;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE