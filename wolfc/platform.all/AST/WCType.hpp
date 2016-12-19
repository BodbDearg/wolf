#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class AssignExpr;
class DataType;
class LinearAlloc;
class PrimitiveType;

/*
Type:
	PrimitiveType
	[ AssignExpr ] Type
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

/* [ AssignExpr ] Type */
class TypeArray final : public Type {
public:
    TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    const Token &   mStartToken;
    AssignExpr &    mSizeExpr;
    Type &          mElemType;
    
    /* The data type of the type array. This is generated during code generation. */
    DataType * mDataType = nullptr;
    
    /* The size of the array. This is evaluated when compiling. */
    size_t mArraySize = 0;
    
    /* True if the array size expression has been evaluated. */
    bool mArraySizeEvaluated = false;
};

WC_END_NAMESPACE
