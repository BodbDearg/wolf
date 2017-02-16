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
    return  currentToken->type == TokenType::kLBrack ||
            PrimitiveType::peek(currentToken);
}

Type * Type::parse(ParseCtx & parseCtx) {
    // See if there is an array type following:
    if (parseCtx.tok()->type == TokenType::kLBrack) {
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
