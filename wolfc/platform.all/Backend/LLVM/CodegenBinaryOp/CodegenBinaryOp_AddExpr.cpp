//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenBinaryOp_AddExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenAddOrSubBinaryOp
//-----------------------------------------------------------------------------
CodegenAddOrSubBinaryOp::CodegenAddOrSubBinaryOp(Codegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr,
                                                 const char * opSymbol,
                                                 const char * opName,
                                                 bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    opSymbol,
                    opName,
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

bool CodegenAddOrSubBinaryOp::verifyLeftAndRightTypesAreOkForOp() {
    // Special case logic for pointer arithmetic:
    const DataType & leftType = mLeftVal.mCompiledType.getDataType();
    
    if (leftType.isPtr()) {
        // Get the right type and left type as a pointer
        const PtrDataType & leftPtrType = static_cast<const PtrDataType&>(leftType);
        const DataType & rightType = mRightVal.mCompiledType.getDataType();
        
        // If this a 'sub' operation it might be pointer distance if the right type is a pointer:
        bool isSubBinaryOp = dynamic_cast<CodegenSubBinaryOp*>(this);
        
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
                    mCG.mCtx.error("Can't perform '%s' (%s) pointer arithmetic op on pointers of type '%s'! "
                                   "The pointed to type must be valid and have a size that is known at "
                                   "compile time in order for pointer arithmetic to be performed!",
                                   mOpSymbol,
                                   mOpName,
                                   leftPtrType.name().c_str());
                    
                    return false;
                }
            }
            else {
                // Pointer difference operation but pointer types are different, issue an error:
                mCG.mCtx.error("Pointed to types for left and right pointers involved in '%s' (%s) "
                               "pointer difference op must be the same! Can't get pointer difference "
                               "when the pointed to types are different! Left side pointer operand is "
                               "of type '%s' and right side pointer operand is of type '%s'!",
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
            mCG.mCtx.error("Can't perform '%s' (%s) pointer arithmetic op on pointers of type '%s'! "
                           "The pointed to type must be valid and have a size that is known at compile "
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
                "Right side of '%s' (%s) pointer arithmetic operation must be an "
                "integer or pointer type not '%s'!" :
                "Right side of '%s' (%s) pointer arithmetic operation must be an "
                "integer type not '%s'!";
            
            mCG.mCtx.error(msgFmtStr,
                           mOpSymbol,
                           mOpName,
                           rightType.name().c_str());
        }
    }
    
    // Normal case, left and right types must match:
    return CodegenBinaryOp::verifyLeftAndRightTypesAreOkForOp();
}

//-----------------------------------------------------------------------------
// CodegenAddBinaryOp
//-----------------------------------------------------------------------------
CodegenAddBinaryOp::CodegenAddBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenAddOrSubBinaryOp(cg,
                            leftExpr,
                            rightExpr,
                            "+",
                            "add",
                            storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int128, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int16, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int32, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int64, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, Int8, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt128, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt16, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt32, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt64, CreateAdd)
WC_IMPL_BASIC_BINARY_OP(CodegenAddBinaryOp, UInt8, CreateAdd)

void CodegenAddBinaryOp::visit(const PtrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    WC_ASSERT(mLeftVal.mLLVMVal);
    WC_ASSERT(mRightVal.mLLVMVal);
    
    llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreateGEP(
        mLeftVal.mLLVMVal,
        mRightVal.mLLVMVal,
        "CodegenAddBinaryOp:Result"
    );

    WC_ASSERT(resultVal);
    pushOpResult(resultVal);
}

//-----------------------------------------------------------------------------
// CodegenSubBinaryOp
//-----------------------------------------------------------------------------
CodegenSubBinaryOp::CodegenSubBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenAddOrSubBinaryOp(cg,
                            leftExpr,
                            rightExpr,
                            "-",
                            "subtract",
                            storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int128, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int16, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int32, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int64, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, Int8, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt128, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt16, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt32, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt64, CreateSub)
WC_IMPL_BASIC_BINARY_OP(CodegenSubBinaryOp, UInt8, CreateSub)

void CodegenSubBinaryOp::visit(const PtrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    
    // See if the right side is a pointer (pointer difference operation)
    const DataType & rightType = mRightVal.mCompiledType.getDataType();
    
    if (rightType.isPtr()) {
        // Pointer difference operation:
        WC_ASSERT(mLeftVal.mLLVMVal);
        WC_ASSERT(mRightVal.mLLVMVal);
        llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreatePtrDiff(mLeftVal.mLLVMVal, mRightVal.mLLVMVal);
        WC_ASSERT(resultVal);
        
        // Get the compiled data type for the result (int)
        const DataType & resultType = PrimitiveDataTypes::getDefaultIntType();
        resultType.accept(mCG.mCodegenDataType);
        CompiledDataType resultCDT = mCG.mCtx.popCompiledDataType();
        
        // Ensure that the pointer difference is in the format of the native int type we are compiling for:
        resultVal = mCG.mCtx.mIRBuilder.CreateTruncOrBitCast(resultVal, resultCDT.getLLVMType());
        WC_ASSERT(resultVal);
        
        // Sanity checks in debug:
        WC_ASSERT(resultCDT.isValid());
        WC_ASSERT(resultCDT.getLLVMType() == resultVal->getType());
        
        // Save the result:
        pushOpResult(resultVal, false, resultCDT);
    }
    else {
        // Regular pointer '-' (subtract) pointer arithmetic.
        // Moves the pointer along by subtracting it whatever number of elements:
        WC_ASSERT(mRightVal.mLLVMVal);
        llvm::Value * rightValNegated = mCG.mCtx.mIRBuilder.CreateNeg(mRightVal.mLLVMVal);
        WC_ASSERT(rightValNegated);
        WC_ASSERT(mLeftVal.mLLVMVal);
        
        llvm::Value * resultVal = mCG.mCtx.mIRBuilder.CreateGEP(
            mLeftVal.mLLVMVal,
            rightValNegated,
            "CodegenSubBinaryOp:Result"
        );
        
        WC_ASSERT(resultVal);
        pushOpResult(resultVal);
    }
}

//-----------------------------------------------------------------------------
// CodegenBOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBOrBinaryOp::CodegenBOrBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "|",
                    "bitwise or",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int128, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int16, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int32, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int64, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, Int8, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt128, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt16, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt32, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt64, CreateOr)
WC_IMPL_BASIC_BINARY_OP(CodegenBOrBinaryOp, UInt8, CreateOr)

//-----------------------------------------------------------------------------
// CodegenBXOrBinaryOp
//-----------------------------------------------------------------------------
CodegenBXOrBinaryOp::CodegenBXOrBinaryOp(Codegen & cg,
                                         const AST::ASTNode & leftExpr,
                                         const AST::ASTNode & rightExpr,
                                         bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    "^",
                    "bitwise xor",
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int128, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int16, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int32, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int64, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, Int8, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt128, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt16, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt32, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt64, CreateXor)
WC_IMPL_BASIC_BINARY_OP(CodegenBXOrBinaryOp, UInt8, CreateXor)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
