#include "CodegenBinaryOp.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "DataType/DataType.hpp"

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
    // Get the type for both left and right expressions:
    const AST::IExpr * leftAsExpr = dynamic_cast<const AST::IExpr*>(&mLeftExpr);
    
    if (!leftAsExpr) {
        mCG.mCtx.error(mLeftExpr,
                       "Left side of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    const AST::IExpr * rightAsExpr = dynamic_cast<const AST::IExpr*>(&mRightExpr);
    
    if (!rightAsExpr) {
        mCG.mCtx.error(mRightExpr,
                       "Right side of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
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
    
    // The left and right types must match:
    const DataType & leftType = mLeftVal.mCompiledType.getDataType();
    const DataType & rightType = mRightVal.mCompiledType.getDataType();
    
    if (!leftType.equals(rightType)) {
        mCG.mCtx.error(*mLeftExpr.mParent,
                       "Left and right side expressions for binary operator '%s' (%s) must be "
                       "of the same type! Left expression type is '%s', right expression type is "
                       "'%s'!",
                       mOpSymbol,
                       mOpName,
                       leftType.name().c_str(),
                       rightType.name().c_str());
        
        return;
    }
    
    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftVal.isValid() && mRightVal.isValid());

    // Code generate the actual operation itself
    leftType.accept(*this);
    
    // See if we are to store the result on the left, if not then we are done.
    // Otherwise, grab the result of the operation:
    WC_GUARD(mStoreResultOnLeft);
    Value opResultVal = mCG.mCtx.popValue();

    // The operator result type must match the left side type
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
}

void CodegenBinaryOp::visit(const ArrayBadSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const ArrayUnevalSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const FuncDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const InvalidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
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
    WC_ASSERT(result);
    mCG.mCtx.pushValue(Value(result, resultType, requiresLoad, mLeftExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
