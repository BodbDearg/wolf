#include "CodegenConstBinaryOpHelper.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "CodegenCtx.hpp"
#include "ConstCodegen.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenConstBinaryOpHelper::CodegenConstBinaryOpHelper(ConstCodegen & cg,
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

void CodegenConstBinaryOpHelper::codegen() {
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
    
    // Okay, codegen both the left and right expressions
    mLeftExpr.accept(mCG);
    mLeftConst = mCG.mCtx.popConstant();
    mRightExpr.accept(mCG);
    mRightConst = mCG.mCtx.popConstant();
    
    // The left and right types must match:
    const DataType & leftType = mLeftConst.mCompiledType.getDataType();
    const DataType & rightType = mRightConst.mCompiledType.getDataType();
    
    // TODO: Support auto type promotion
    if (leftType.equals(rightType)) {
        if (mLeftConst.isValid() && mRightConst.isValid()) {
            leftType.accept(*this);
        }
    }
    else {
        mCG.mCtx.error(*mLeftExpr.mParent,
                       "Left and right side expressions for binary operator '%s' (%s) must be "
                       "of the same type! Left expression type is '%s', right expression type is "
                       "'%s'!",
                       mOpSymbol,
                       mOpName,
                       leftType.name().c_str(),
                       rightType.name().c_str());
    }
}

void CodegenConstBinaryOpHelper::visit(const ArrayBadSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const ArrayUnevalSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const UnknownDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueBinaryOpNotSupportedError();
}

void CodegenConstBinaryOpHelper::issueBinaryOpNotSupportedError() {
    AST::ASTNode * parent = mLeftExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Binary operator '%s' (%s) is not supported for a left expression "
                   "of type '%s' and right expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mLeftConst.mCompiledType.getDataType().name().c_str(),
                   mRightConst.mCompiledType.getDataType().name().c_str());
}

void CodegenConstBinaryOpHelper::pushOpResult(llvm::Constant * result) {
    pushOpResult(result, mLeftConst.mCompiledType);
}

void CodegenConstBinaryOpHelper::pushOpResult(llvm::Constant * result, const CompiledDataType & resultType) {
    WC_ASSERT(result);
    mCG.mCtx.pushConstant(Constant(result, resultType, mLeftExpr.mParent));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
