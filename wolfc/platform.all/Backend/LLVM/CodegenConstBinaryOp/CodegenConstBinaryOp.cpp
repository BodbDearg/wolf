//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstBinaryOp.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenConstBinaryOp::CodegenConstBinaryOp(ConstCodegen & cg,
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

void CodegenConstBinaryOp::codegen() {
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

#define WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(DataTypeName)\
    void CodegenConstBinaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        issueBinaryOpNotSupportedError();\
    }

WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Array)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Bool)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Func)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int128)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int16)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int32)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int64)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int8)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Invalid)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Ptr)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Str)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt128)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt16)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt32)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt64)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt8)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Void)

void CodegenConstBinaryOp::issueBinaryOpNotSupportedError() {
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

void CodegenConstBinaryOp::pushOpResult(llvm::Constant * result) {
    pushOpResult(result, mLeftConst.mCompiledType);
}

void CodegenConstBinaryOp::pushOpResult(llvm::Constant * result, const CompiledDataType & resultType) {
    WC_ASSERT(result);
    mCG.mCtx.pushConstant(Constant(result, resultType, mLeftExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
