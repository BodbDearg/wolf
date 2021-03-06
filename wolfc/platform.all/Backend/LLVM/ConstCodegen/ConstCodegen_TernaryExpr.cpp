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
#include "Assert.hpp"
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
    
    // Do any implicit type conversions between the two types that may be required
    ImplicitCasts::castBinaryOpValuesIfRequired(*this, trueVal, falseVal);
    
    // The condition value must be of type bool
    bool condValIsBool = true;
    
    {
        const DataType & condValType = condConst.mCompiledType.getDataType();
        
        if (!condValType.isBool()) {
            // Note: no error msg in the case of 'undefined' since that means an error was triggered elsewhere
            if (!condValType.isUndefined()) {
                mCtx.error(astNode.mCondExpr,
                           "Condition expression for ternary operator must be of type 'bool', not '%s'!",
                           condValType.name().c_str());
            }
            
            condValIsBool = false;
        }
    }
    
    // The data types for the two expressions must be non void (sized):
    bool trueAndFalseValsAreSized = true;
    const DataType & trueValType = trueVal.mCompiledType.getDataType();
    
    if (!trueValType.isSized()) {
        // Note: no error msg in the case of 'undefined' since that means an error was triggered elsewhere
        if (!trueValType.isUndefined()) {
            mCtx.error(astNode,
                       "The 'true' expression value in a ternary operator must be of a valid "
                       "sized data type not '%s'!",
                       trueValType.name().c_str());
        }
        
        trueAndFalseValsAreSized = false;
    }
    
    const DataType & falseValType = falseVal.mCompiledType.getDataType();
    
    if (!falseValType.isSized()) {
        // Note: no error msg in the case of 'undefined' since that means an error was triggered elsewhere
        if (!falseValType.isUndefined()) {
            mCtx.error(astNode,
                       "The 'false' expression value in a ternary operator must be of a valid "
                       "sized data type not '%s'!",
                       falseValType.name().c_str());
        }
        
        trueAndFalseValsAreSized = false;
    }
    
    // The two expressions must match in terms of type:
    bool trueFalseValsTypeMatch = true;
    
    if (!trueValType.equals(falseValType)) {
        // Note: no error msg in the case of 'undefined' since that means an error was triggered elsewhere
        if (!trueValType.isUndefined() && !falseValType.isUndefined()) {
            mCtx.error(astNode,
                       "The 'true' and 'false' expressions of the ternary operator must be of the same type! "
                       "The true expression is of type '%s' while the false expression is of type '%s'!",
                       trueValType.name().c_str(),
                       falseValType.name().c_str());
        }
        
        trueFalseValsTypeMatch = false;
    }
    
    // Proceed no further if any of these checks fail
    WC_GUARD(condValIsBool);
    WC_GUARD(trueAndFalseValsAreSized);
    WC_GUARD(trueFalseValsTypeMatch);
    WC_GUARD(condConst.isValid());
    WC_GUARD(trueVal.isValid());
    WC_GUARD(falseVal.isValid());
    
    // Okay see which value to use:
    WC_ASSERT(condConst.mLLVMConst);
    
    if (condConst.mLLVMConst->isZeroValue()) {
        mCtx.pushConstant(Constant(falseVal.mLLVMConst, falseVal.mCompiledType, &astNode));
    }
    else {
        mCtx.pushConstant(Constant(trueVal.mLLVMConst, trueVal.mCompiledType, &astNode));
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
