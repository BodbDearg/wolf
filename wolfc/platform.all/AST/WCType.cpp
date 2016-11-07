#include "WCType.hpp"
#include "WCArrayDataType.hpp"
#include "WCAssignExpr.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCPrimitiveType.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END

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

DataType & TypePrimitive::dataType() {
    return mType.dataType();
}

bool TypePrimitive::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    return mType.codegenLLVMType(cgCtx, callingNode);
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

DataType & TypeArray::dataType() {
    // See if we have evaluated the data type yet, if so then return it:
    if (mDataType) {
        return *mDataType;
    }
    
    // Don't know what our exact data type is yet:
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
}

bool TypeArray::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    // First codegen the element type:
    WC_GUARD(mElemType.codegenLLVMType(cgCtx, callingNode), false);
    
    // The element type must have a size:
    if (!mElemType.dataType().isSized()) {
        compileError("Invalid element type for array: '%s'! Arrays can only contain valid sized elements.",
                     mElemType.dataType().name().c_str());
        
        return false;
    }
    
    // Next codegen the assign expression for the array size:
    llvm::Constant * arraySize = mSizeExpr.codegenExprConstEval(cgCtx);
    
    // Okay, so the array size must be an integer:
    if (llvm::ConstantInt * arraySizeAsInt = llvm::dyn_cast<llvm::ConstantInt>(arraySize)) {
        if (arraySizeAsInt->isNegative()) {
            compileError("Size expression for array must be a valid unsigned integer!");
            return false;
        }
       
        const auto & sizeAPInt = arraySizeAsInt->getValue();
        
        if (sizeAPInt.getNumWords() > 1) {
            compileError("Array size is too big for array! Max supported size: %zu",
                         UINT64_MAX);
            
            return false;
        }
        
        // Save the array size:
        mArraySize = sizeAPInt.getZExtValue();
        mArraySizeEvaluated = true;
    }
    else {
        compileError("Size expression for array must be a valid unsigned integer!");
        return false;
    }
    
    // TODO: what manages memory here?
    // Now create a data type for the array:
    mDataType = new ArrayDataType(mElemType.dataType(), mArraySize);
    
    // Codegen the array data type so we can get the llvm type later:
    WC_GUARD(mDataType->codegenLLVMTypeIfRequired(cgCtx, *this), false);
    
    // If we have reached here then we are all good!
    return true;
}

WC_END_NAMESPACE
