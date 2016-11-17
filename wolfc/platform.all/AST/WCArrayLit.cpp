#include "WCArrayLit.hpp"

#include "WCArrayLitExprs.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
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
    mDataType(),
    mStorage(nullptr)
{
    mExprs.mParent = this;
}

const Token & ArrayLit::getStartToken() const {
    return mLBrack;
}

const Token & ArrayLit::getEndToken() const {
    return mRBrack;
}

bool ArrayLit::isLValue() {
    return true;
}

bool ArrayLit::isConstExpr() {
    return mExprs.isConstExpr();
}

DataType & ArrayLit::dataType() {
    if (!mDataType.wasInitialized()) {
        mDataType.init(mExprs.getElementType(), mSize);
    }
    
    return mDataType.get();
}

llvm::Value * ArrayLit::getStorage() const {
    return mStorage;
}

void ArrayLit::setStorage(llvm::Value & storage) {
    WC_ASSERT(!mStorage);
    mStorage = &storage;
}

llvm::Value * ArrayLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * ArrayLit::codegenExprEval(CodegenCtx & cgCtx) {
    // Generate the code for the element type:
    WC_GUARD(codegenLLVMType(cgCtx), nullptr);
    
    // Allocate stack space for the array if we need to:
    if (!mStorage) {
        llvm::AllocaInst * storage = cgCtx.irBuilder.CreateAlloca(mDataType->mLLVMType);
        WC_ASSERT(storage);
        setStorage(*storage);
    }
    
    // Evaluate the array element expressions:
    std::vector<AssignExpr*> exprs;
    mExprs.getExprs(exprs);
    std::vector<llvm::Value*> exprValues;
    exprValues.reserve(exprs.size());
    
    for (size_t i = 0; i < exprs.size(); ++i) {
        // Codegen the expression:
        llvm::Value * exprValue = exprs[i]->codegenExprEval(cgCtx);
        WC_GUARD(exprValue, nullptr);
        exprValues.push_back(exprValue);
    }
    
    // Assign each of the element expressions to the array:
    for (size_t i = 0; i < exprs.size(); ++i) {
        // Figure out the pointer to the array element:
        llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), 0);
        WC_ASSERT(zeroIndex);
        llvm::ConstantInt * arrayIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), i);
        WC_ASSERT(arrayIndex);
        llvm::Value * arrayElemPtr = cgCtx.irBuilder.CreateGEP(mStorage, { zeroIndex, arrayIndex });
        WC_ASSERT(arrayElemPtr);
        
        // TODO: this probably will not work for complex types and nested arrays
        // Do the actual store to the array element.
        llvm::Value * storeResult = cgCtx.irBuilder.CreateStore(exprValues[i], arrayElemPtr);
        WC_ASSERT(storeResult);
    }
    
    return cgCtx.irBuilder.CreateLoad(mStorage);
}

llvm::Constant * ArrayLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Generate the code for the element type:
    WC_GUARD(codegenLLVMType(cgCtx), nullptr);
    
    // Get the sub expressions
    std::vector<AssignExpr*> subExprs;
    mExprs.getExprs(subExprs);
    
    // Alright Generate the code for each of the sub expressions
    std::vector<llvm::Constant*> subexprConstants;
    subexprConstants.reserve(subExprs.size());
    
    for (AssignExpr * subExpr : subExprs) {
        llvm::Constant * subExprConstant = subExpr->codegenExprConstEval(cgCtx);
        WC_GUARD(subExprConstant, nullptr);
        subexprConstants.push_back(subExprConstant);
    }
    
    // Now create a constant array and return
    return llvm::ConstantArray::get(static_cast<llvm::ArrayType*>(mDataType->mLLVMType), subexprConstants);
}

bool ArrayLit::codegenLLVMType(CodegenCtx & cgCtx) {
    // Need to lazy init the data type
    ArrayDataType & arrayDataType = static_cast<ArrayDataType&>(dataType());
    
    // Element type checks:
    if (arrayDataType.mInnerType.isUnknown()) {
        compileError("Unable to determine element type for array! "
                     "Element type is ambiguous since different elements have different types!");
        
        return false;
    }
    
    if (!arrayDataType.mInnerType.isSized()) {
        compileError("Invalid element type for array: '%s'! Array element types must be sized.",
                     arrayDataType.name().c_str());
        
        return false;
    }

    // Generate the code for the element:
    WC_GUARD(arrayDataType.codegenLLVMTypeIfRequired(cgCtx, *this), nullptr);
    
    // Verify all is good:
    if (!arrayDataType.mLLVMType) {
        compileError("Invalid element type for array! Unable to determine the llvm type.");
        return false;
    }
    
    return true;    // Success!
}

WC_END_NAMESPACE
