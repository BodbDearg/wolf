#pragma once

#include "WCASTNode.hpp"
#include "WCUnknownArrayDataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstddef>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PrimitiveType;

/*
Type:
	PrimitiveType
	[ AssignExpr ] Type
*/
class Type : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static Type * parse(ParseCtx & parseCtx);
    
    virtual const DataType & dataType() = 0;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
#endif
};

/* PrimitiveType */
class TypePrimitive final : public Type {
public:
    TypePrimitive(PrimitiveType & type);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
#endif
    
    PrimitiveType & mType;
};

/* [ AssignExpr ] Type */
class TypeArray final : public Type {
public:
    TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual const DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
#endif
    
    const Token &   mStartToken;
    AssignExpr &    mSizeExpr;
    Type &          mElemType;
    
    /**
     * The data type of the type array. 
     * It has to converted to a concrete array type during compiling.
     */
    UnknownArrayDataType mDataType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
