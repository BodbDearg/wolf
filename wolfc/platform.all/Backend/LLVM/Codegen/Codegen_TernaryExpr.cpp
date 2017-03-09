//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/LOrExpr.hpp"
#include "AST/Nodes/TernaryExpr.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::TernaryExprNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::TernaryExprWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Evalute the condition expression
    astNode.mCondExpr.accept(*this);
    Value condVal = mCtx.popValue();
    
    // Implicitly convert the condition expression to bool if we have to or can
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    condVal = ImplicitCasts::castSingleValueIfRequired(*this, condVal, boolCDT);
    
    // The condition value must be of type bool
    bool condValIsBool = true;
    
    {
        const DataType & condValType = condVal.mCompiledType.getDataType();
        
        if (!condValType.isBool()) {
            mCtx.error(astNode.mCondExpr,
                       "Condition expression for ternary operator must be of type 'bool', not '%s'!",
                       condValType.name().c_str());
            
            condValIsBool = false;
        }
    }
    
    // Get the current function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a block for the true, false and follow on expressions:
    std::string trueBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:true", astNode.mTrueExpr.getStartToken());
    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, trueBBLbl, parentFn);
    
    std::string falseBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:false", astNode.mFalseExpr.getStartToken());
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, falseBBLbl, parentFn);
    
    std::string endBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:end", astNode.getPastEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLbl, parentFn);
    
    // Evaluate the true expression value and the false expression value
    mCtx.pushInsertBlock();
    
    mCtx.mIRBuilder.SetInsertPoint(trueBB);
    astNode.mTrueExpr.accept(*this);
    Value trueVal = mCtx.popValue();
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateBr(endBB));
    
    mCtx.mIRBuilder.SetInsertPoint(falseBB);
    astNode.mFalseExpr.accept(*this);
    Value falseVal = mCtx.popValue();
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateBr(endBB));
    
    mCtx.popInsertBlock();
    
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
    WC_GUARD(condVal.isValid());
    WC_GUARD(trueVal.isValid());
    WC_GUARD(falseVal.isValid());
    
    // Generate the code for the condition branch:
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, trueBB, falseBB));
    
    // All code after that goes into the end block:
    mCtx.mIRBuilder.SetInsertPoint(endBB);
    
    // Create a PHI node to merge the values:
    llvm::PHINode * phiNode = mCtx.mIRBuilder.CreatePHI(trueVal.mCompiledType.getLLVMType(), 2, "TernaryExprWithCond:PHI");
    WC_ASSERT(phiNode);
    phiNode->addIncoming(trueVal.mLLVMVal, trueBB);
    phiNode->addIncoming(falseVal.mLLVMVal, falseBB);
    
    // The PHI node is the result of the operation:
    mCtx.pushValue(Value(phiNode, trueVal.mCompiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
