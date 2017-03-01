//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PrimitiveType;

/*
Type:
	PrimitiveType
	^|? Type
	[ AssignExpr ] Type
*/
class Type : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static Type * parse(ParseCtx & parseCtx);
    
    /* Tell if this type is the 'any' data type */
    virtual bool isAny() const;
};

/* PrimitiveType */
class TypePrimitive final : public Type {
public:
    TypePrimitive(PrimitiveType & type);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isAny() const override;
    
    PrimitiveType & mType;
};

/* ^|? Type */
class TypePtr final : public Type {
public:
    TypePtr(const Token & startToken, Type & pointedToType);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    bool isNullablePtr() const;
    
    const Token &   mStartToken;
    Type &          mPointedToType;
};

/* [ AssignExpr ] Type */
class TypeArray final : public Type {
public:
    TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const Token &   mStartToken;
    AssignExpr &    mSizeExpr;
    Type &          mElemType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
