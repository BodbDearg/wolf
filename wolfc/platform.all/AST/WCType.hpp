#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
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
    
    virtual DataType & dataType() = 0;
    
    virtual bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
};

/* PrimitiveType */
class TypePrimitive : public Type {
public:
    TypePrimitive(PrimitiveType & type);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    PrimitiveType & mType;
};

/* Type [ AssignExpr ] */
class TypeArray : public Type {
public:
    TypeArray(Type & elemType, AssignExpr & sizeExpr, const Token & endBracket);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    Type &          mElemType;
    AssignExpr &    mSizeExpr;
    const Token &   mEndBracket;
    
    /* The data type of the type array. This is generated during code generation. */
    DataType * mDataType = nullptr;
    
    /* The size of the array. This is evaluated when compiling. */
    size_t mArraySize = 0;
    
    /* True if the array size expression has been evaluated. */
    bool mArraySizeEvaluated = false;
};

WC_END_NAMESPACE
