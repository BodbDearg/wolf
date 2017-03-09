//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenLazyLogicalBinaryOp.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "ImplicitCasts.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenLazyLogicalBinaryOp::CodegenLazyLogicalBinaryOp(Codegen & cg,
                                                       const AST::ASTNode & leftExpr,
                                                       const AST::ASTNode & rightExpr,
                                                       OpType opType)
:
    mCG(cg),
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr),
    mOpType(opType)
{
    // Expected that the nodes have a parent and that it is the same parent! (parent binary op)
    WC_ASSERT(mLeftExpr.mParent);
    WC_ASSERT(mRightExpr.mParent);
    WC_ASSERT(mLeftExpr.mParent == mRightExpr.mParent);
}

void CodegenLazyLogicalBinaryOp::codegen() {
    // Save the start basic block which we start off in as well as the parent function
    llvm::BasicBlock * startBB = mCG.mCtx.mIRBuilder.GetInsertBlock();
    WC_ASSERT(startBB);
    llvm::Function * parentFn = startBB->getParent();
    WC_ASSERT(parentFn);
    
    // The op symbol and name:
    const char * opSymbol = mOpType == OpType::kAnd ? "and" : "or";
    const char * opName = mOpType == OpType::kAnd ? "logical and" : "logical or";
    
    // Get the expression for both the left and right operands:
    const AST::IExpr * leftAsExpr = dynamic_cast<const AST::IExpr*>(&mLeftExpr);
    
    if (!leftAsExpr) {
        mCG.mCtx.error(mLeftExpr,
                       "Left side of '%s' (%s) operator must be an expression with a value result!",
                       opSymbol,
                       opName);
    }
    
    const AST::IExpr * rightAsExpr = dynamic_cast<const AST::IExpr*>(&mRightExpr);
    
    if (!rightAsExpr) {
        mCG.mCtx.error(mRightExpr,
                       "Right side of '%s' (%s) operator must be an expression with a value result!",
                       opSymbol,
                       opName);
    }
    
    // Firstly codegen the left expression. Note: expect the result to not require a load.
    mLeftExpr.accept(mCG);
    Value leftVal = mCG.mCtx.popValue();
    WC_ASSERT(!leftVal.mRequiresLoad || !leftVal.isValid());
    
    // Codegen the bool data type and do any implicit promotion to bool required for the left value
    PrimitiveDataTypes::getBoolDataType().accept(mCG.mCodegenDataType);
    CompiledDataType boolCDT = mCG.mCtx.popCompiledDataType();
    ImplicitCasts::castSingleValueIfRequired(mCG, leftVal, boolCDT);
    
    // Save the basic block which the left value came from, we'll need it later for a PHI node.
    // Since there could be multiple sub expressions and operations on the left side, this
    // might not be the same as the start block for evaluating the left side...
    llvm::BasicBlock * evalLeftEndBB = mCG.mCtx.mIRBuilder.GetInsertBlock();
    
    // Okay, create a basic block for starting the evaluation of the right side
    std::string evalRightStartBBLbl = StringUtils::appendLineInfo(
        mOpType == OpType::kAnd ? "EvalLAndRightExpr" : "EvalLOrRightExpr",
        mRightExpr.getStartToken()
    );
    
    llvm::BasicBlock * evalRightStartBB = llvm::BasicBlock::Create(mCG.mCtx.mLLVMCtx,
                                                                   evalRightStartBBLbl,
                                                                   parentFn);
    
    // Evaluate the right expression in this basic block. Note: expect the result to not require a load.
    mCG.mCtx.pushInsertBlock();
    mCG.mCtx.mIRBuilder.SetInsertPoint(evalRightStartBB);
    mRightExpr.accept(mCG);
    Value rightVal = mCG.mCtx.popValue();
    WC_ASSERT(!rightVal.mRequiresLoad || !rightVal.isValid());
    
    // Do any implicit promotion to bool required for the right value:
    ImplicitCasts::castSingleValueIfRequired(mCG, rightVal, boolCDT);
    
    // Don't do anything more if either side is not valid:
    WC_GUARD(leftVal.isValid() && rightVal.isValid());
    
    // The left and right expression values must be booleans:
    const DataType & leftType = leftVal.mCompiledType.getDataType();
    const DataType & rightType = rightVal.mCompiledType.getDataType();
    
    if (!leftType.isBool() || !rightType.isBool()) {
        mCG.mCtx.error(*mLeftExpr.mParent,
                       "Left and right side expressions for binary operator '%s' (%s) must be "
                       "of type 'bool'! Left expression type is '%s', right expression type is "
                       "'%s'!",
                       opSymbol,
                       opName,
                       leftType.name().c_str(),
                       rightType.name().c_str());
        
        return;
    }
    
    // Save the basic block which the right value came from, we'll need it later for a PHI node.
    // Since there could be multiple sub expressions and operations on the right side, this
    // might not be the same as the start block for evaluating the right side...
    llvm::BasicBlock * evalRightEndBB = mCG.mCtx.mIRBuilder.GetInsertBlock();
    
    // Create the end basic block, this will be where we figure out the result of the expression
    std::string evalEndBBLbl = StringUtils::appendLineInfo(
        mOpType == OpType::kAnd ? "EvalLAndEnd" : "EvalLOrEnd",
        mRightExpr.getPastEndToken()
    );
    
    llvm::BasicBlock * evalEndBB = llvm::BasicBlock::Create(mCG.mCtx.mLLVMCtx,
                                                            evalEndBBLbl,
                                                            parentFn);
    
    // Switch back to the previous insert block and do the branch for the lazy logical
    // evaluation depending on the result of the left side:
    mCG.mCtx.popInsertBlock();
    
    if (mOpType == OpType::kAnd) {
        // Logical AND:
        // Only evaluate the right side if the left side is true.
        mCG.mCtx.mIRBuilder.CreateCondBr(leftVal.mLLVMVal, evalRightStartBB, evalEndBB);
    }
    else {
        // Logical OR:
        // Only evaluate the right side if the left side is not true.
        mCG.mCtx.mIRBuilder.CreateCondBr(leftVal.mLLVMVal, evalEndBB, evalRightStartBB);
    }
    
    // Switch to the end basic block, where we will compute the result of the expression
    mCG.mCtx.mIRBuilder.SetInsertPoint(evalEndBB);
    
    // Create a PHI node to merge the possible incoming values and compute the result of the expression:
    const char * phiNodeLbl = mOpType == OpType::kAnd ? "EvalLAndPHI" : "EvalLOrPHI";
    llvm::PHINode * phiNode = mCG.mCtx.mIRBuilder.CreatePHI(leftVal.mCompiledType.getLLVMType(), 2, phiNodeLbl);
    WC_ASSERT(phiNode);
    phiNode->addIncoming(leftVal.mLLVMVal, evalLeftEndBB);
    phiNode->addIncoming(rightVal.mLLVMVal, evalRightEndBB);
    
    // The result is the PHI node:
    WC_ASSERT(mLeftExpr.mParent);
    mCG.mCtx.pushValue(Value(phiNode, leftVal.mCompiledType, false, mLeftExpr.mParent));
    
    // Final step: link the end block for the right side evaluation back to the end block where we
    // evaluate the overall result. This will work for several chained logical statements also...
    mCG.mCtx.pushInsertBlock();
    mCG.mCtx.mIRBuilder.SetInsertPoint(evalRightEndBB);
    mCG.mCtx.mIRBuilder.CreateBr(evalEndBB);
    mCG.mCtx.popInsertBlock();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
