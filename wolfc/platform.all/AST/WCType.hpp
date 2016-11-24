#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class PrimitiveType;
class TernaryExpr;
struct CodegenCtx;

/*
Type:
	PrimitiveType
	Type [ TernaryExpr ]
*/
class Type : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static Type * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    virtual DataType & dataType() = 0;
    
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
};

/* PrimitiveType */
class TypePrimitive final : public Type {
public:
    TypePrimitive(PrimitiveType & type);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    PrimitiveType & mType;
};

/* Type [ TernaryExpr ] */
class TypeArray final : public Type {
public:
    TypeArray(Type & elemType, TernaryExpr & sizeExpr, const Token & endBracket);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    Type &          mElemType;
    TernaryExpr &   mSizeExpr;
    const Token &   mEndBracket;
    
    /* The data type of the type array. This is generated during code generation. */
    DataType * mDataType = nullptr;
    
    /* The size of the array. This is evaluated when compiling. */
    size_t mArraySize = 0;
    
    /* True if the array size expression has been evaluated. */
    bool mArraySizeEvaluated = false;
};

WC_END_NAMESPACE
