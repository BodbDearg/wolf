#pragma once

#include "WCASTNode.hpp"

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
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    DataType & dataType();
    
#warning FIXME - Codegen
#if 0
    bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode);
#endif
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
