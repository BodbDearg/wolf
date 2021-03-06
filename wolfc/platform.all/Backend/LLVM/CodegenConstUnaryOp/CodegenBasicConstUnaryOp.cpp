//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenBasicConstUnaryOp.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenBasicConstUnaryOp::CodegenBasicConstUnaryOp(ConstCodegen & cg,
                                                   const AST::ASTNode & expr,
                                                   const char * opSymbol,
                                                   const char * opName)
:
    mCG(cg),
    mExpr(expr),
    mOpSymbol(opSymbol),
    mOpName(opName)
{
    // Expected that the node has a parent!
    WC_ASSERT(mExpr.mParent);
}

void CodegenBasicConstUnaryOp::codegen() {
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (!nodeAsExpr) {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the operand expression
    mExpr.accept(mCG);
    mExprConst = mCG.mCtx.popConstant();

    // Must have a generated type and value to proceed any further:
    WC_GUARD(mExprConst.isValid());
    
    // Do any type conversions that are required:
    doAnyImplicitTypeCastingRequiredForExprConst();
    
    // Now codegen the operation for the expression type:
    const DataType & exprType = mExprConst.mCompiledType.getDataType();
    exprType.accept(*this);
}

#define WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(DataTypeName)\
    void CodegenBasicConstUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        issueUnaryOpNotSupportedError();\
    }

WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Any)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Array)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Bool)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Func)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int128)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int16)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int32)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int64)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int8)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Nullptr)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Ptr)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Str)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt128)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt16)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt32)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt64)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt8)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Undefined)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Void)

void CodegenBasicConstUnaryOp::issueUnaryOpNotSupportedError() {
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprConst.mCompiledType.getDataType().name().c_str());
}

void CodegenBasicConstUnaryOp::doAnyImplicitTypeCastingRequiredForExprConst() {
    // The default impl of this is just a no-op...
    WC_EMPTY_FUNC_BODY();
}

void CodegenBasicConstUnaryOp::pushOpResult(llvm::Constant * result) {
    pushOpResult(result, mExprConst.mCompiledType);
}

void CodegenBasicConstUnaryOp::pushOpResult(llvm::Constant * result, const CompiledDataType & resultType) {
    WC_ASSERT(result);
    mCG.mCtx.pushConstant(Constant(result, resultType, mExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
