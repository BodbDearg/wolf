#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class LinearAlloc;
class PrimitiveType;

/*
Type:
	PrimitiveType
	Type [ AssignExpr ]
*/
class Type : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static Type * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    virtual const DataType & dataType() const = 0;
};

/* PrimitiveType */
class TypePrimitive : public Type {
public:
    TypePrimitive(PrimitiveType & type);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    PrimitiveType & mType;
};

/* Type [ AssignExpr ] */
class TypeArray : public Type {
public:
    TypeArray(Type & elemType, AssignExpr & sizeExpr, const Token & endBracket);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() const override;
    
    Type &          mElemType;
    AssignExpr &    mSizeExpr;
    const Token &   mEndBracket;
};

WC_END_NAMESPACE
