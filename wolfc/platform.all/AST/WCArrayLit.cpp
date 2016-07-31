#include "WCArrayLit.hpp"
#include "WCArrayLitExprs.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool ArrayLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kLBrack;
}

ArrayLit * ArrayLit::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial '[':
    if (tokenPtr->type != TokenType::kLBrack) {
        parseError(*tokenPtr, "Expected integer literal!");
        return nullptr;
    }
    
    const Token * lBrack = tokenPtr;
    ++tokenPtr;
    
    // Parse the array literal expressions
    ArrayLitExprs * exprs = ArrayLitExprs::parse(tokenPtr, alloc);
    WC_GUARD(exprs, nullptr);
    
    // Parse the closing ']'
    if (tokenPtr->type != TokenType::kRBrack) {
        parseError(*tokenPtr, "Expected closing ']' for array literal!");
        return nullptr;
    }
    
    const Token * rBrack = tokenPtr;
    ++tokenPtr;
    
    // Now return the parsed node:
    return WC_NEW_AST_NODE(alloc, ArrayLit, *lBrack, *exprs, *rBrack);
}

ArrayLit::ArrayLit(const Token & lBrack,
                   ArrayLitExprs & exprs,
                   const Token & rBrack)
:
    mLBrack(lBrack),
    mExprs(exprs),
    mRBrack(rBrack),
    mSize(exprs.numExprs()),
    mElementType(exprs.getElementType(), mSize)
{
    mExprs.mParent = this;
}

const Token & ArrayLit::getStartToken() const {
    return mLBrack;
}

const Token & ArrayLit::getEndToken() const {
    return mRBrack;
}

bool ArrayLit::isLValue() const {
    return false;
}

DataType & ArrayLit::dataType() {
    return mExprs.getElementType();
}

llvm::Value * ArrayLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * ArrayLit::codegenExprEval(CodegenCtx & cgCtx) {
    // Generate the code for the element type:
    WC_GUARD(codegenElementType(cgCtx), nullptr);
    
    // Alloc room on the stack for the array:
    llvm::Type * arraySizeLLVMTy = llvm::Type::getInt64Ty(cgCtx.llvmCtx);
    WC_ASSERT(arraySizeLLVMTy);
    llvm::AllocaInst * allocInst = cgCtx.irBuilder.CreateAlloca(mElementType.mLLVMType,
                                                                llvm::ConstantInt::get(arraySizeLLVMTy, mSize));
    
    // TODO: actually fill in the elements
    #warning TODO: fill in array elements
    
    // Return the array
    return allocInst;
}

llvm::Constant * ArrayLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Generate the code for the element type:
    WC_GUARD(codegenElementType(cgCtx), nullptr);
    
    /*
    //
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), mToken.data.intVal);
    */
    #warning TODO: array constant code generation
    return nullptr;
}

bool ArrayLit::codegenElementType(CodegenCtx & cgCtx) {
    // Element type checks:
    if (mElementType.isUnknown()) {
        compileError("Unable to determine element type for array! "
                     "Element type is ambiguous since different elements have different types!");
        
        return false;
    }
    
    if (!mElementType.isSized()) {
        compileError("Invalid element type for array: '%s'! Array element types must be sized.",
                     mElementType.name().c_str());
        
        return false;
    }

    // Generate the code for the element:
    WC_GUARD(mElementType.codegen(cgCtx, *this), nullptr);
    
    // Verify all is good:
    if (!mElementType.mLLVMType) {
        compileError("Invalid element type for array! Unable to determine the llvm type.");
        return false;
    }
    
    return true;    // Success!
}

WC_END_NAMESPACE
