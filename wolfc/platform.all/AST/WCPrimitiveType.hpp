#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class DataType;
class LinearAlloc;

/*
PrimitiveType:
	void|int|bool|string
*/
class PrimitiveType : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static PrimitiveType * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    PrimitiveType(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    DataType & dataType();
    
    bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode);
    
    const Token & mToken;
};

WC_END_NAMESPACE
