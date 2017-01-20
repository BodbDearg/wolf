#include "WCType.hpp"

#include "AST/Nodes/WCPrimitiveType.hpp"
#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/Primitives/WCArrayDataType.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

#warning FIXME - Codegen
#if 0
WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END
#endif

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

DataType & TypePrimitive::dataType() {
    return mType.dataType();
}

#warning FIXME - Codegen
#if 0
bool TypePrimitive::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    return mType.codegenLLVMType(cgCtx, callingNode);
}
#endif

//-----------------------------------------------------------------------------
// TypeArray
//-----------------------------------------------------------------------------
TypeArray::TypeArray(const Token & startToken, AssignExpr & sizeExpr, Type & elemType) :
    mStartToken(startToken),
    mSizeExpr(sizeExpr),
    mElemType(elemType),
    mDataType(elemType.dataType(), sizeExpr)
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

DataType & TypeArray::dataType() {
    return mDataType;
}

#warning FIXME - Codegen
#if 0
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
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
