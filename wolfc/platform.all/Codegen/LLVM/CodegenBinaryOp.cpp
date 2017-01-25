#include "CodegenBinaryOp.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "Codegen.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenBinaryOp::CodegenBinaryOp(Codegen & cg,
                                 const AST::ASTNode & leftExpr,
                                 const AST::ASTNode & rightExpr,
                                 const char * opSymbol,
                                 const char * opName)
:
    mCG(cg),
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr),
    mOpSymbol(opSymbol),
    mOpName(opName)
{
    // Expected that the nodes have a parent and that it is the same parent! (parent binary op)
    WC_ASSERT(mLeftExpr.mParent);
    WC_ASSERT(mRightExpr.mParent);
    WC_ASSERT(mLeftExpr.mParent == mRightExpr.mParent);
}

void CodegenBinaryOp::codegen() {
    // Get the type for both left and right expressions:
    const AST::IExpr * leftAsExpr = dynamic_cast<const AST::IExpr*>(&mLeftExpr);
    
    if (leftAsExpr) {
        mLeftType = &leftAsExpr->dataType();
    }
    else {
        mCG.mCtx.error(mLeftExpr,
                       "Left side of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    const AST::IExpr * rightAsExpr = dynamic_cast<const AST::IExpr*>(&mRightExpr);
    
    if (rightAsExpr) {
        mRightType = &rightAsExpr->dataType();
    }
    else {
        mCG.mCtx.error(mRightExpr,
                       "Right side of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen both the left and right expressions
    mLeftExpr.accept(mCG);
    mLeftVal = mCG.mCtx.popLLVMValue();
    mRightExpr.accept(mCG);
    mRightVal = mCG.mCtx.popLLVMValue();
    
    // The left and right types must match:
    WC_GUARD(mLeftType && mRightType);
    
    if (mLeftType->equals(*mRightType)) {
        if (mLeftVal && mRightVal) {
            mLeftType->accept(*this);
        }
    }
    else {
        mCG.mCtx.error(*mLeftExpr.mParent,
                       "Left and right side expressions for binary operator '%s' (%s) must be "
                       "of the same type! Left expression type is '%s', right expression type is "
                       "'%s'!",
                       mOpSymbol,
                       mOpName,
                       mLeftType->name().c_str(),
                       mRightType->name().c_str());
    }
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

void CodegenBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const UnknownDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenBinaryOp::issueBinaryOpNotSupportedError() {
    WC_GUARD(mLeftType && mRightType);
    AST::ASTNode * parent = mLeftExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Binary operator '%s' (%s) is not supported for a left expression "
                   "of type '%s' and right expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mLeftType->name().c_str(),
                   mRightType->name().c_str());
}

void CodegenBinaryOp::pushOpResult(llvm::Value * result) {
    WC_GUARD_ASSERT(result);
    mCG.mCtx.pushLLVMValue(*result);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
