//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstLogicalBinaryOp.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenConstLogicalBinaryOp::CodegenConstLogicalBinaryOp(ConstCodegen & cg,
                                                         const AST::ASTNode & leftExpr,
                                                         const AST::ASTNode & rightExpr,
                                                         const char * opSymbol,
                                                         const char * opName)
:
    Wolfc::LLVMBackend::CodegenConstBinaryOp(cg,
                                             leftExpr,
                                             rightExpr,
                                             opSymbol,
                                             opName)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstLogicalBinaryOp::codegen() {
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
    
    // Codegen the bool data type that is the result data type for the logical op
    PrimitiveDataTypes::getBoolDataType().accept(mCG.mCodegenDataType);
    CompiledDataType boolCDT = mCG.mCtx.popCompiledDataType();
    
    // Do automatic type promotion to bool if required/allowed:
    ImplicitCasts::castSingleConstantIfRequired(mCG, mLeftConst, boolCDT);
    ImplicitCasts::castSingleConstantIfRequired(mCG, mRightConst, boolCDT);

    // Don't do anything if either side is not valid:
    WC_GUARD(mLeftConst.isValid() && mRightConst.isValid());
    
    #warning WHAT if both types are not bool?
    // The left and right types must match:
    WC_GUARD(verifyLeftAndRightTypesAreOkForOp());
    
    // Codegen the operation
    const DataType & leftType = mLeftConst.mCompiledType.getDataType();
    leftType.accept(*this);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
