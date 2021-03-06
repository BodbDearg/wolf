//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstBinaryOp.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"

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
    // Sanity check, left and right operands must be expressions:
    WC_ASSERT(dynamic_cast<const AST::IExpr*>(&mLeftExpr));
    WC_ASSERT(dynamic_cast<const AST::IExpr*>(&mRightExpr));
    
    // Okay, codegen both the left and right expressions
    mLeftExpr.accept(mCG);
    mLeftConst = mCG.mCtx.popConstant();
    mRightExpr.accept(mCG);
    mRightConst = mCG.mCtx.popConstant();
    
    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftConst.isValid() && mRightConst.isValid());
    
    // Do any automatic type promotion required and allowed:
    ImplicitCasts::castBinaryOpValuesIfRequired(mCG, mLeftConst, mRightConst);

    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftConst.isValid() && mRightConst.isValid());
    
    // The left and right types must match:
    WC_GUARD(verifyLeftAndRightTypesAreOkForOp());
    
    // Codegen the operation
    const DataType & leftType = mLeftConst.mCompiledType.getDataType();
    leftType.accept(*this);
}

#define WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(DataTypeName)\
    void CodegenConstBinaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        issueBinaryOpNotSupportedError();\
    }

WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Any)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Array)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Bool)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Func)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int128)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int16)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int32)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int64)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Int8)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Nullptr)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Ptr)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Str)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt128)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt16)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt32)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt64)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt8)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Undefined)
WC_IMPL_CONST_BINARY_OP_NOT_SUPPORTED_FOR_TYPE(Void)

bool CodegenConstBinaryOp::verifyLeftAndRightTypesAreOkForOp() {
    // By default left and right types must match:
    const DataType & leftType = mLeftConst.mCompiledType.getDataType();
    const DataType & rightType = mRightConst.mCompiledType.getDataType();
    
    if (!leftType.equals(rightType)) {
        issueBinaryOpNotSupportedError();
        return false;
    }
    
    return true;    // All is good!
}

void CodegenConstBinaryOp::issueBinaryOpNotSupportedError() {
    AST::ASTNode * parent = mLeftExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Compile time binary operator '%s' (%s) is not supported for a left expression "
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
