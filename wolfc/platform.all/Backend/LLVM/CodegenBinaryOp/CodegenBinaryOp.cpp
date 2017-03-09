//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenBinaryOp.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/VoidDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenBinaryOp::CodegenBinaryOp(Codegen & cg,
                                 const AST::ASTNode & leftExpr,
                                 const AST::ASTNode & rightExpr,
                                 const char * opSymbol,
                                 const char * opName,
                                 bool storeResultOnLeft)
:
    mCG(cg),
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr),
    mOpSymbol(opSymbol),
    mOpName(opName),
    mStoreResultOnLeft(storeResultOnLeft)
{
    // Expected that the nodes have a parent and that it is the same parent! (parent binary op)
    WC_ASSERT(mLeftExpr.mParent);
    WC_ASSERT(mRightExpr.mParent);
    WC_ASSERT(mLeftExpr.mParent == mRightExpr.mParent);
}

void CodegenBinaryOp::codegen() {
    // Sanity check, left and right operands must be expressions:
    WC_ASSERT(dynamic_cast<const AST::IExpr*>(&mLeftExpr));
    WC_ASSERT(dynamic_cast<const AST::IExpr*>(&mRightExpr));
    
    // Okay, codegen both the left and right expressions.
    // If we want to store the result on the left, codegen the address of the variable instead of it's value.
    if (mStoreResultOnLeft) {
        mLeftExpr.accept(mCG.mAddrCodegen);
    }
    else {
        mLeftExpr.accept(mCG);
    }
    
    Value leftValBeforeLoad = mCG.mCtx.popValue();
    
    if (mStoreResultOnLeft) {
        // Storing the result on the left, create the load if we can
        if (leftValBeforeLoad.isValid()) {
            // Since we are storing on the left, we expect there to be a load required.
            WC_ASSERT(leftValBeforeLoad.mRequiresLoad);
            llvm::Value * leftValLoaded = mCG.mCtx.mIRBuilder.CreateLoad(leftValBeforeLoad.mLLVMVal, "BinaryOp:LeftValLoaded");
            WC_ASSERT(leftValLoaded);
            mLeftVal = Value(leftValLoaded, leftValBeforeLoad.mCompiledType, false, leftValBeforeLoad.mDeclaringNode);
        }
    }
    else {
        // Not storing the result on the left, expect the result to not require a load
        mLeftVal = leftValBeforeLoad;
        WC_ASSERT(!mLeftVal.mRequiresLoad || !mLeftVal.isValid());
    }
    
    // Note: we expect the right expression to not require a load ever
    mRightExpr.accept(mCG);
    mRightVal = mCG.mCtx.popValue();
    WC_ASSERT(!mRightVal.mRequiresLoad || !mRightVal.isValid());
    
    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftVal.isValid() && mRightVal.isValid());
    
    // Do any automatic type promotion required and allowed.
    // If we are storing the result on the left, then we do a basic 1-way cast.
    // If we are doing a binary with no store, either value can potentially cast to the other...
    if (mStoreResultOnLeft) {
        ImplicitCasts::castSingleValueIfRequired(mCG, mRightVal, mLeftVal.mCompiledType);
    }
    else {
        ImplicitCasts::castBinaryOpValuesIfRequired(mCG, mLeftVal, mRightVal);
    }
    
    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftVal.isValid() && mRightVal.isValid());
    
    // Make sure the left and right types are valid:
    WC_GUARD(verifyLeftAndRightTypesAreOkForOp());

    // Code generate the actual operation itself
    const DataType & leftType = mLeftVal.mCompiledType.getDataType();
    leftType.accept(*this);
    
    // See if we are to store the result on the left, if not then we are done.
    // Otherwise, grab the result of the operation:
    WC_GUARD(mStoreResultOnLeft);
    Value opResultVal = mCG.mCtx.popValue();

    // The operator result type must match the left side type.
    //
    // TODO: Someday lift this restriction for user defined operator overloads.
    const DataType & opResultType = opResultVal.mCompiledType.getDataType();
    
    if (!opResultType.equals(leftType)){
        mCG.mCtx.error(*mLeftExpr.mParent,
                       "Result of binary op can't be assigned to left side due to mismatched types! "
                       "Require binary op result to be of type '%s'! Result of binary op was type '%s'!",
                       leftType.name().c_str(),
                       opResultType.name().c_str());
        
        return;
    }
    
    // If the operator result is not valid then continue no further
    WC_GUARD(opResultVal.isValid());
    
    // All good, do the actual store:
    WC_ASSERT(!opResultVal.mRequiresLoad);
    WC_ASSERTED_OP(mCG.mCtx.mIRBuilder.CreateStore(opResultVal.mLLVMVal, leftValBeforeLoad.mLLVMVal));
    
    // The result of a stored operation is 'void'
    PrimitiveDataTypes::getVoidDataType().accept(mCG.mCodegenDataType);
    CompiledDataType voidCDT = mCG.mCtx.popCompiledDataType();
    pushOpResult(nullptr, false, voidCDT);
}

#define WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(DataTypeName)\
    void CodegenBinaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        issueBinaryOpNotSupportedError();\
    }

WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Any)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Array)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Bool)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Func)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int128)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int16)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int32)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int64)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int8)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Invalid)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Nullptr)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Ptr)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Str)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt128)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt16)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt32)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt64)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt8)
WC_IMPL_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Void)

bool CodegenBinaryOp::verifyLeftAndRightTypesAreOkForOp() {
    // By default the left and right types must match exactly:
    const DataType & leftType = mLeftVal.mCompiledType.getDataType();
    const DataType & rightType = mRightVal.mCompiledType.getDataType();
    
    if (!leftType.equals(rightType)) {
        issueBinaryOpNotSupportedError();
        return false;
    }
    
    return true;    // All is good!
}

void CodegenBinaryOp::issueBinaryOpNotSupportedError() {
    AST::ASTNode * parent = mLeftExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Binary operator '%s' (%s) is not supported for a left expression "
                   "of type '%s' and right expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mLeftVal.mCompiledType.getDataType().name().c_str(),
                   mRightVal.mCompiledType.getDataType().name().c_str());
}

void CodegenBinaryOp::pushOpResult(llvm::Value * result) {
    pushOpResult(result, false);
}

void CodegenBinaryOp::pushOpResult(llvm::Value * result,
                                   bool requiresLoad)
{
    pushOpResult(result, requiresLoad, mLeftVal.mCompiledType);
}

void CodegenBinaryOp::pushOpResult(llvm::Value * result,
                                   bool requiresLoad,
                                   const CompiledDataType & resultType)
{
    mCG.mCtx.pushValue(Value(result, resultType, requiresLoad, mLeftExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
