#include "CodegenUnaryOp.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/VoidDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenUnaryOp::CodegenUnaryOp(Codegen & cg,
                               const AST::ASTNode & expr,
                               const char * opSymbol,
                               const char * opName,
                               bool storeResultInExpr)
:
    mCG(cg),
    mExpr(expr),
    mOpSymbol(opSymbol),
    mOpName(opName),
    mStoreResultInExpr(storeResultInExpr)
{
    // Expected that the node has a parent!
    WC_ASSERT(mExpr.mParent);
}

void CodegenUnaryOp::codegen() {
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (!nodeAsExpr) {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the operand expression.
    // If we want to store the result, codegen the address of the variable instead of it's value.
    if (mStoreResultInExpr) {
        mExpr.accept(mCG.mAddrCodegen);
    }
    else {
        mExpr.accept(mCG);
    }
    
    Value exprValBeforeLoad = mCG.mCtx.popValue();
    
    if (mStoreResultInExpr) {
        // Storing the result in the operand, create the load if we can
        if (exprValBeforeLoad.isValid()) {
            // Since we are storing on the left, we expect there to be a load required.
            WC_ASSERT(exprValBeforeLoad.mRequiresLoad);
            llvm::Value * exprValLoaded = mCG.mCtx.mIRBuilder.CreateLoad(exprValBeforeLoad.mLLVMVal, "UnaryOp:OperandLoaded");
            WC_ASSERT(exprValLoaded);
            mExprVal = Value(exprValLoaded, exprValBeforeLoad.mCompiledType, false, exprValBeforeLoad.mDeclaringNode);
        }
    }
    else {
        // Not storing the result on the left, expect the result to not require a load
        mExprVal = exprValBeforeLoad;
        WC_ASSERT(!mExprVal.mRequiresLoad || !mExprVal.isValid());
    }
    
    // Don't do anything the expression value is not valid:
    WC_GUARD(mExprVal.isValid());
    
    // Code generate the actual operation itself
    const DataType & exprType = mExprVal.mCompiledType.getDataType();
    exprType.accept(*this);
    
    // See if we are to store the result in the expression, if not then we are done.
    // Otherwise, grab the result of the operation:
    WC_GUARD(mStoreResultInExpr);
    Value opResultVal = mCG.mCtx.popValue();
    
    // The operator result type must match the type of the variable we are storing to
    const DataType & opResultType = opResultVal.mCompiledType.getDataType();
    
    if (!opResultType.equals(exprType)) {
        mCG.mCtx.error(*mExpr.mParent,
                       "Result of unary op can't be assigned to the operand due to mismatched types! "
                       "Require unary op result to be of type '%s'! Result of unary op was type '%s'!",
                       exprType.name().c_str(),
                       opResultType.name().c_str());
        
        return;
    }
    
    // If the operator result is not valid then continue no further
    WC_GUARD(opResultVal.isValid());

    // All good, do the actual store:
    WC_ASSERT(!opResultVal.mRequiresLoad);
    WC_ASSERTED_OP(mCG.mCtx.mIRBuilder.CreateStore(opResultVal.mLLVMVal, exprValBeforeLoad.mLLVMVal));
    
    // The result of a stored operation is 'void'
    PrimitiveDataTypes::getVoidDataType().accept(mCG.mCodegenDataType);
    CompiledDataType voidCDT = mCG.mCtx.popCompiledDataType();
    pushOpResult(nullptr, false, voidCDT);
}

void CodegenUnaryOp::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const FuncDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const InvalidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::issueUnaryOpNotSupportedError() {
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprVal.mCompiledType.getDataType().name().c_str());
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result) {
    pushOpResult(result, false);
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result, bool requiresLoad) {
    pushOpResult(result, requiresLoad, mExprVal.mCompiledType);
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result, bool requiresLoad, const CompiledDataType & resultType) {
    mCG.mCtx.pushValue(Value(result, resultType, requiresLoad, mExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
