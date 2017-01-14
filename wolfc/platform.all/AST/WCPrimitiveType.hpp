#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
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
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    DataType & dataType();
    
    bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode);
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
