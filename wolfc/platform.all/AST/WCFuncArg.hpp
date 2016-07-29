#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class Identifier;
class LinearAlloc;
class Type;

/*
FuncArg:
	PrimitiveType Identifier
*/
class FuncArg : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static FuncArg * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    FuncArg(Type & dataType, Identifier & ident);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    const DataType & dataType() const;
    
    const char * name() const;
    
    Type &          mType;
    Identifier &    mIdent;
};

WC_END_NAMESPACE
