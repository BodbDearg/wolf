//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/*
PrimitiveType:
	void
	any
	nullptr
	int|int8|int16|int32|int64|int128
	uint|uint8|uint16|uint32|uint64|uint128
	bool
	string
*/
class PrimitiveType final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static PrimitiveType * parse(ParseCtx & parseCtx);
    
    PrimitiveType(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    /* Tell if this type is the 'any' data type */
    bool isAny() const;
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
