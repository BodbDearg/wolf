#include "WCType.hpp"
#include "WCAssignExpr.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveType.hpp"
#include "WCToken.hpp"

#warning TODO: remove
#include "WCPrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Type
//-----------------------------------------------------------------------------

bool Type::peek(const Token * currentToken) {
    return PrimitiveType::peek(currentToken);
}

Type * Type::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // Parse a primitive type first:
    PrimitiveType * primitiveType = PrimitiveType::parse(currentToken, alloc);
    WC_GUARD(primitiveType, nullptr);
    
    // Make the outer type:
    Type * outerType = WC_NEW_AST_NODE(alloc, TypePrimitive, *primitiveType);
    
    // Continue parsing until we see no array etc. tokens ahead:
    while (true) {
        // See if array specifier ahead: [ AssignExpr ]
        if (currentToken->type == TokenType::kLBrack) {
            // Skip the '['
            ++currentToken;
            
            // Parse the inner assign expression for the array size:
            AssignExpr * arraySizeExpr = AssignExpr::parse(currentToken, alloc);
            WC_GUARD(arraySizeExpr, nullptr);
            
            // Expect a ']' next:
            if (currentToken->type != TokenType::kRBrack) {
                parseError(*currentToken, "Expected ']' to close array size specifier!");
                return nullptr;
            }
            
            // Consume the ']':
            const Token * endBracket = currentToken;
            ++currentToken;
            
            // Make the new array type, and make it the new outer type:
            outerType = WC_NEW_AST_NODE(alloc, TypeArray, *outerType, *arraySizeExpr, * endBracket);
            
            // Continue parsing for now:
            continue;
        }
        
        // If we've got to here then nothing else is ahead, break:
        break;
    }
    
    // Return the outer type:
    return outerType;
}

//-----------------------------------------------------------------------------
// TypePrimitive
//-----------------------------------------------------------------------------

TypePrimitive::TypePrimitive(PrimitiveType & type) : mType(type) {
    mParent = this;
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

TypeArray::TypeArray(Type & elemType, AssignExpr & sizeExpr, const Token & endBracket) :
    mElemType(elemType),
    mSizeExpr(sizeExpr),
    mEndBracket(endBracket)
{
    mElemType.mParent = this;
    mSizeExpr.mParent = this;
}

const Token & TypeArray::getStartToken() const {
    return mElemType.getStartToken();
}

const Token & TypeArray::getEndToken() const {
    return mEndBracket;
}

const DataType & TypeArray::dataType() const {
    #warning TODO: fix this
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
}

WC_END_NAMESPACE
