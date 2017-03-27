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
    ImplicitCasts::castSingleValueIfRequired(*this, condVal, boolCDT);
    
    // The condition value must be of type bool
    bool condValIsBool = true;
    
    {
        const DataType & condValType = condVal.mCompiledType.getDataType();
        
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
    
    // Get the current function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a basic block for the true and false follow on expressions:
    std::string trueBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:true", astNode.mTrueExpr.getStartToken());
    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, trueBBLbl, parentFn);
    WC_ASSERT(trueBB);
    
    std::string falseBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:false", astNode.mFalseExpr.getStartToken());
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, falseBBLbl, parentFn);
    WC_ASSERT(falseBB);
    
    // Need to save the current insert block so we can return to it later
    mCtx.pushInsertBlock();
    
    // Evaluate the true expression value and the false expression value.
    // Note: save the end basic blocks for both expressions since they might differ (may have lazy eval for instance)
    mCtx.mIRBuilder.SetInsertPoint(trueBB);
    astNode.mTrueExpr.accept(*this);
    Value trueVal = mCtx.popValue();
    llvm::BasicBlock * trueEndBB = mCtx.mIRBuilder.GetInsertBlock();
    WC_ASSERT(trueEndBB);
    
    mCtx.mIRBuilder.SetInsertPoint(falseBB);
    astNode.mFalseExpr.accept(*this);
    Value falseVal = mCtx.popValue();
    llvm::BasicBlock * falseEndBB = mCtx.mIRBuilder.GetInsertBlock();
    WC_ASSERT(falseEndBB);
    
    // Determine if we should do implicit casts so the type of both expressions
    // matches, and if we should cast in which direction:
    ImplicitCasts::BinaryOpCastDir castDir = ImplicitCasts::determineCastDirForBinaryOp(mCtx, trueVal, falseVal);
    
    if (castDir == ImplicitCasts::BinaryOpCastDir::kCastLeftToRight) {
        mCtx.mIRBuilder.SetInsertPoint(trueEndBB);
        ImplicitCasts::castSingleValueIfRequired(*this, trueVal, falseVal.mCompiledType);
    }
    else if (castDir == ImplicitCasts::BinaryOpCastDir::kCastRightToLeft) {
        mCtx.mIRBuilder.SetInsertPoint(falseEndBB);
        ImplicitCasts::castSingleValueIfRequired(*this, falseVal, trueVal.mCompiledType);
    }
    
    // Create the end basic block:
    std::string endBBLbl = StringUtils::appendLineInfo("TernaryExprWithCond:end", astNode.getPastEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Create the branches at the end of the true and false basic blocks to the end basic block
    mCtx.mIRBuilder.SetInsertPoint(trueEndBB);
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateBr(endBB));
    mCtx.mIRBuilder.SetInsertPoint(falseEndBB);
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateBr(endBB));
    
    // Return to the previously saved insert block
    mCtx.popInsertBlock();
    
    // The two expressions must match in terms of type:
    bool trueFalseValsTypeMatch = true;
    
    {
        const DataType & trueValType = trueVal.mCompiledType.getDataType();
        const DataType & falseValType = falseVal.mCompiledType.getDataType();
        
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
    }
    
    // Proceed no further if any of these checks fail
    WC_GUARD(condValIsBool);
    WC_GUARD(trueFalseValsTypeMatch);
    WC_GUARD(condVal.isValid());
    WC_GUARD(trueVal.isValid());
    WC_GUARD(falseVal.isValid());
    
    // Generate the code for the condition branch:
    WC_ASSERT(condVal.mLLVMVal);
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, trueBB, falseBB));
    
    // All code after that goes into the end block:
    mCtx.mIRBuilder.SetInsertPoint(endBB);
    
    // Create a PHI node to merge the values:
    llvm::PHINode * phiNode = mCtx.mIRBuilder.CreatePHI(trueVal.mCompiledType.getLLVMType(), 2, "TernaryExprWithCond:PHI");
    WC_ASSERT(phiNode);
    #warning VOID - Does this work?
    WC_ASSERT(trueVal.mLLVMVal);
    WC_ASSERT(falseVal.mLLVMVal);
    phiNode->addIncoming(trueVal.mLLVMVal, trueEndBB);
    phiNode->addIncoming(falseVal.mLLVMVal, falseEndBB);
    
    // The PHI node is the result of the operation:
    mCtx.pushValue(Value(phiNode, trueVal.mCompiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
