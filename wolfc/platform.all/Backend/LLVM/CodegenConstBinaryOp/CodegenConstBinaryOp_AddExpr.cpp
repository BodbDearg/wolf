//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstBinaryOp_AddExpr.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenConstAddOrSubBinaryOp
//-----------------------------------------------------------------------------
CodegenConstAddOrSubBinaryOp::CodegenConstAddOrSubBinaryOp(ConstCodegen & cg,
                                                           const AST::ASTNode & leftExpr,
                                                           const AST::ASTNode & rightExpr,
                                                           const char * opSymbol,
                                                           const char * opName)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, opSymbol, opName)
{
    WC_EMPTY_FUNC_BODY();
}

bool CodegenConstAddOrSubBinaryOp::verifyLeftAndRightTypesAreOkForOp() {
    // Special case logic for pointer arithmetic:
    const DataType & leftType = mLeftConst.mCompiledType.getDataType();
    
    if (leftType.isPtr()) {
        // Get the right type and left type as a pointer
        const PtrDataType & leftPtrType = static_cast<const PtrDataType&>(leftType);
        const DataType & rightType = mRightConst.mCompiledType.getDataType();
        
        // If this a 'sub' operation it might be pointer distance if the right type is a pointer:
        bool isSubBinaryOp = dynamic_cast<CodegenConstSubBinaryOp*>(this);
        
        if (isSubBinaryOp && rightType.isPtr()) {
            // Dealing with pointer difference operation. Pointed to types must be the same:
            const PtrDataType & rightPtrType = static_cast<const PtrDataType&>(rightType);
            const DataType & leftPointedToType = leftPtrType.mPointedToType;
            const DataType & rightPointedToType =  rightPtrType.mPointedToType;
            
            if (leftPointedToType.equals(rightPointedToType)) {
                // All good, pointer difference operation where the left and right pointer types match.
                // For the last check we must ensure that the types pointed to are valid and sized.
                if (leftPointedToType.isValid() && leftPointedToType.isSized()) {
                    // All is well with this operation:
                    return true;
                }
                else {
                    // Can't do pointer operation on unknown sized or invalid type:
                    mCG.mCtx.error("Can't perform '%s' (%s) compile time pointer arithmetic op on pointers of "
                                   "type '%s'! The pointed to type must be valid and have a size that is "
                                   "known at compile time in order for pointer arithmetic to be performed!",
                                   mOpSymbol,
                                   mOpName,
                                   leftPtrType.name().c_str());
                    
                    return false;
                }
            }
            else {
                // Pointer difference operation but pointer types are different, issue an error:
                mCG.mCtx.error("Pointed to types for left and right pointers involved in '%s' (%s) "
                               "compile time pointer difference op must be the same! Can't get pointer "
                               "difference when the pointed to types are different! Left side pointer "
                               "operand is of type '%s' and right side pointer operand is of type '%s'!",
                               mOpSymbol,
                               mOpName,
                               leftPtrType.name().c_str(),
                               rightPtrType.name().c_str());
            }
        }
        
        // Dealing with pointer arithmetic. Left side must firstly be a pointer to a valid type for
        // us to be able to figure out pointer offsets:
        const DataType & pointedToType = leftPtrType.mPointedToType;
        
        if (!pointedToType.isValid() || !pointedToType.isSized()) {
            mCG.mCtx.error("Can't perform '%s' (%s) compile time pointer arithmetic op on pointers of type "
                           "'%s'! The pointed to type must be valid and have a size that is known at compile "
                           "time in order for pointer arithmetic to be performed!",
                           mOpSymbol,
                           mOpName,
                           leftPtrType.name().c_str());
            
            return false;
        }

        // Okay, now make sure the right type is a valid integer type:
        if (rightType.isInteger()) {
            return true;
        }
        else {
            const char * msgFmtStr = isSubBinaryOp ?
                "Right side of '%s' (%s) compile time pointer arithmetic operation must be an "
                "integer or pointer type not '%s'!" :
                "Right side of '%s' (%s) compile time pointer arithmetic operation must be an "
                "integer type not '%s'!";
            
            mCG.mCtx.error(msgFmtStr,
                           mOpSymbol,
                           mOpName,
                           rightType.name().c_str());
        }
    }
    
    // Normal case, left and right types must match:
    return CodegenConstBinaryOp::verifyLeftAndRightTypesAreOkForOp();
}

//-----------------------------------------------------------------------------
// CodegenConstAddBinaryOp
//-----------------------------------------------------------------------------
CodegenConstAddBinaryOp::CodegenConstAddBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstAddOrSubBinaryOp(cg, leftExpr, rightExpr, "+", "add")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int128, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int16, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int32, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int64, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, Int8, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt128, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt16, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt32, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt64, getAdd)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstAddBinaryOp, UInt8, getAdd)

void CodegenConstAddBinaryOp::visit(const PtrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    WC_ASSERT(mLeftConst.mLLVMConst);
    WC_ASSERT(mRightConst.mLLVMConst);
    
    llvm::Constant * resultConst = llvm::ConstantExpr::getGetElementPtr(
        mLeftConst.mCompiledType.getLLVMType()->getContainedType(0),
        mLeftConst.mLLVMConst,
        mRightConst.mLLVMConst
    );
    
    WC_ASSERT(resultConst);
    pushOpResult(resultConst);
}

//-----------------------------------------------------------------------------
// CodegenConstSubBinaryOp
//-----------------------------------------------------------------------------
CodegenConstSubBinaryOp::CodegenConstSubBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstAddOrSubBinaryOp(cg, leftExpr, rightExpr, "-", "subtract")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int128, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int16, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int32, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int64, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, Int8, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt128, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt16, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt32, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt64, getSub)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstSubBinaryOp, UInt8, getSub)

void CodegenConstSubBinaryOp::visit(const PtrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    
    // See if the right side is a pointer (pointer difference operation)
    const DataType & rightType = mRightConst.mCompiledType.getDataType();
    
    if (rightType.isPtr()) {
        // Get the compiled data type for the result (int)
        const DataType & resultType = PrimitiveDataTypes::getDefaultIntType();
        resultType.accept(mCG.mCodegenDataType);
        CompiledDataType resultCDT = mCG.mCtx.popCompiledDataType();
        
        // Pointer difference operation: convert the two pointers to integers firstly:
        llvm::Type * llvmIntTy = resultCDT.getLLVMType();
        WC_ASSERT(llvmIntTy);
        
        WC_ASSERT(mLeftConst.mLLVMConst);
        llvm::Constant * ptr1AsInt = llvm::ConstantExpr::getPtrToInt(mLeftConst.mLLVMConst, llvmIntTy);
        WC_ASSERT(ptr1AsInt);
        
        WC_ASSERT(mRightConst.mLLVMConst);
        llvm::Constant * ptr2AsInt = llvm::ConstantExpr::getPtrToInt(mRightConst.mLLVMConst, llvmIntTy);
        WC_ASSERT(ptr2AsInt);
        
        // Then get the difference between them as an integer:
        llvm::Constant * ptrDiff = llvm::ConstantExpr::getSub(ptr1AsInt, ptr2AsInt);
        WC_ASSERT(ptrDiff);
        
        // Divide by the size of the type pointed to:
        llvm::Type * typePointedTo = mLeftConst.mCompiledType.getLLVMType()->getContainedType(0);
        WC_ASSERT(typePointedTo);
        uint64_t typePointedToSize = mCG.mCtx.mLLVMModule->getDataLayout().getTypeStoreSize(typePointedTo);
        WC_ASSERT(typePointedToSize > 0);
        
        // The result is the diff divided by the size:
        llvm::Constant * typePointedToSizeConst = llvm::ConstantInt::get(llvmIntTy, typePointedToSize);
        WC_ASSERT(typePointedToSizeConst);
        llvm::Constant * resultConst = llvm::ConstantExpr::getSDiv(ptrDiff, typePointedToSizeConst);
        WC_ASSERT(resultConst);
        
        // Sanity checks in debug:
        WC_ASSERT(resultCDT.isValid());
        WC_ASSERT(resultCDT.getLLVMType() == resultConst->getType());
        
        // Save the result:
        pushOpResult(resultConst, resultCDT);
    }
    else {
        // Regular pointer '-' (subtract) pointer arithmetic.
        // Moves the pointer along by subtracting it whatever number of elements:
        WC_ASSERT(mRightConst.mLLVMConst);
        llvm::Constant * rightConstNegated = llvm::ConstantExpr::getNeg(mRightConst.mLLVMConst);
        WC_ASSERT(rightConstNegated);
        WC_ASSERT(mLeftConst.mLLVMConst);
        
        llvm::Constant * resultConst = llvm::ConstantExpr::getGetElementPtr(
            mLeftConst.mCompiledType.getLLVMType()->getContainedType(0),
            mLeftConst.mLLVMConst,
            rightConstNegated
        );
        
        WC_ASSERT(resultConst);
        pushOpResult(resultConst);
    }
}

//-----------------------------------------------------------------------------
// CodegenConstBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBOrBinaryOp::CodegenConstBOrBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "|", "bitwise or")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int128, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int16, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int32, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int64, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, Int8, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt128, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt16, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt32, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt64, getOr)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBOrBinaryOp, UInt8, getOr)

//-----------------------------------------------------------------------------
// CodegenConstBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenConstBXOrBinaryOp::CodegenConstBXOrBinaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & leftExpr,
                                                   const AST::ASTNode & rightExpr)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, "^", "bitwise xor")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int128, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int16, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int32, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int64, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, Int8, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt128, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt16, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt32, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt64, getXor)
WC_IMPL_BASIC_CONST_BINARY_OP(CodegenConstBXOrBinaryOp, UInt8, getXor)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
