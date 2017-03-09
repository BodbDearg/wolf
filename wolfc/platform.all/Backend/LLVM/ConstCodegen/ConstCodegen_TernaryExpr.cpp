//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/LOrExpr.hpp"
#include "AST/Nodes/TernaryExpr.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::TernaryExprNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::TernaryExprWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Evaluate the condition expression
    astNode.mCondExpr.accept(*this);
    Constant condConst = mCtx.popConstant();
    
    // Implicitly convert the condition expression to bool if we have to or can
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    ImplicitCasts::castSingleConstantIfRequired(*this, condConst, boolCDT);
    
    // Evaluate the left and right expressions
    astNode.mTrueExpr.accept(*this);
    Constant trueVal = mCtx.popConstant();
    astNode.mFalseExpr.accept(*this);
    Constant falseVal = mCtx.popConstant();
    
    // The condition value must be of type bool
    bool condValIsBool = true;
    
    {
        const DataType & condValType = condConst.mCompiledType.getDataType();
        
        if (!condValType.isBool()) {
            mCtx.error(astNode.mCondExpr,
                       "Condition expression for ternary operator must be of type 'bool', not '%s'!",
                       condValType.name().c_str());
            
            condValIsBool = false;
        }
    }
    
    // The two expressions must match in terms of type:
    bool trueFalseValsTypeMatch = true;
    
    {
        const DataType & trueValType = trueVal.mCompiledType.getDataType();
        const DataType & falseValType = falseVal.mCompiledType.getDataType();
        
        if (!trueValType.equals(falseValType)) {
            mCtx.error(astNode,
                       "The 'true' and 'false' expressions of the ternary operator must be of the same type! "
                       "The true expression is of type '%s' while the false expression is of type '%s'!",
                       trueValType.name().c_str(),
                       falseValType.name().c_str());
            
            trueFalseValsTypeMatch = false;
        }
    }
    
    // Proceed no further if any of these checks fail
    WC_GUARD(condValIsBool);
    WC_GUARD(trueFalseValsTypeMatch);
    WC_GUARD(condConst.isValid());
    WC_GUARD(trueVal.isValid());
    WC_GUARD(falseVal.isValid());
    
    // Okay see which value to use:
    if (condConst.mLLVMConst->isZeroValue()) {
        mCtx.pushConstant(Constant(falseVal.mLLVMConst, falseVal.mCompiledType, &astNode));
    }
    else {
        mCtx.pushConstant(Constant(trueVal.mLLVMConst, trueVal.mCompiledType, &astNode));
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
