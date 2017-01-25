#include "WCType.hpp"

#include "AST/Nodes/WCPrimitiveType.hpp"
#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/Primitives/WCArrayDataType.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

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
        // Array type ahead: skip the '['
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the inner assign expression for the array size:
        AssignExpr * arraySizeExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(arraySizeExpr, nullptr);
        
        // Expect a ']' next:
        if (parseCtx.tok()->type != TokenType::kRBrack) {
            parseCtx.error("Expected ']' to close array size specifier!");
            return nullptr;
        }
        
        // Skip ']'
        parseCtx.nextTok();
        
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

const DataType & TypePrimitive::dataType() const {
    return mType.dataType();
}

//-----------------------------------------------------------------------------
// TypeArray
//-----------------------------------------------------------------------------
TypeArray::TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType) :
    mStartToken(startToken),
    mSizeExpr(sizeExpr),
    mElemType(elemType),
    mDataType(*this, elemType.dataType(), sizeExpr)
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

const DataType & TypeArray::dataType() const {
    return mDataType;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
