//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Type.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "PrimitiveType.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Type
//-----------------------------------------------------------------------------
bool Type::peek(const Token * currentToken) {
    TokenType currentTokenType = currentToken->type;
    return  currentTokenType == TokenType::kHat ||
            currentTokenType == TokenType::kQMark ||
            currentTokenType == TokenType::kLBrack ||
            PrimitiveType::peek(currentToken);
}

Type * Type::parse(ParseCtx & parseCtx) {
    // See if there is a pointer type following:
    TokenType currentTokType = parseCtx.tok()->type;
    
    if (currentTokType == TokenType::kHat || currentTokType == TokenType::kQMark) {
        // Array type ahead: skip the '^' or '?' and any newlines that follow
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Parse the type pointed to:
        Type * pointedToType = Type::parse(parseCtx);
        WC_GUARD(pointedToType, nullptr);
        
        // Create the pointer type:
        return WC_NEW_AST_NODE(parseCtx, TypePtr, *startToken, *pointedToType);
    }
    
    // See if there is an array type following:
    if (currentTokType == TokenType::kLBrack) {
        // Array type ahead: skip the '[' and any newlines that follow
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Parse the inner assign expression for the array size.
        // Also skip any newlines following it:
        AssignExpr * arraySizeExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(arraySizeExpr, nullptr);
        parseCtx.skipNewlines();
        
        // Expect a ']' next:
        if (parseCtx.tok()->type != TokenType::kRBrack) {
            parseCtx.error("Expected ']' to close array size specifier!");
            return nullptr;
        }
        
        // Skip ']' and any newlines that follow
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Parse the inner type following:
        Type * innerType = Type::parse(parseCtx);
        WC_GUARD(innerType, nullptr);
        
        // Return the array type:
        return WC_NEW_AST_NODE(parseCtx, TypeArray, *startToken, *arraySizeExpr, *innerType);
    }
    
    // Primitive type following, parse it:
    PrimitiveType * primitiveType = PrimitiveType::parse(parseCtx);
    WC_GUARD(primitiveType, nullptr);
    
    // Return the node parsed
    return WC_NEW_AST_NODE(parseCtx, TypePrimitive, *primitiveType);
}

bool Type::isAny() const {
    return false;   // False for all types by default
}

//-----------------------------------------------------------------------------
// TypePrimitive
//-----------------------------------------------------------------------------
TypePrimitive::TypePrimitive(PrimitiveType & type) : mType(type) {
    mParent = this;
}

void TypePrimitive::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TypePrimitive::getStartToken() const {
    return mType.getStartToken();
}

const Token & TypePrimitive::getEndToken() const {
    return mType.getEndToken();
}

bool TypePrimitive::isAny() const {
    return mType.isAny();
}

//-----------------------------------------------------------------------------
// TypePtr
//-----------------------------------------------------------------------------
TypePtr::TypePtr(const Token & startToken, Type & pointedToType) :
    mStartToken(startToken),
    mPointedToType(pointedToType)
{
    mPointedToType.mParent = this;
}

void TypePtr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TypePtr::getStartToken() const {
    return mStartToken;
}

const Token & TypePtr::getEndToken() const {
    return mPointedToType.getEndToken();
}

bool TypePtr::isNullablePtr() const {
    return mStartToken.type == TokenType::kQMark;
}

//-----------------------------------------------------------------------------
// TypeArray
//-----------------------------------------------------------------------------
TypeArray::TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType) :
    mStartToken(startToken),
    mSizeExpr(sizeExpr),
    mElemType(elemType)
{
    mSizeExpr.mParent = this;
    mElemType.mParent = this;
}

void TypeArray::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TypeArray::getStartToken() const {
    return mStartToken;
}

const Token & TypeArray::getEndToken() const {
    return mElemType.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
