#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class DataType;
class Identifier;
class LinearAlloc;
class Type;

/*
FuncArg:
	PrimitiveType Identifier
*/
class FuncArg final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static FuncArg * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    FuncArg(Type & dataType, Identifier & ident);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    DataType & dataType();
    
    const char * name() const;
    
    bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode);
    
    Type &          mType;
    Identifier &    mIdent;
};

WC_END_NAMESPACE
