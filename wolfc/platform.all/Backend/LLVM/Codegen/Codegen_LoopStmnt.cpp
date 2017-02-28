#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../RepeatableStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/LoopStmnt.hpp"
#include "AST/Nodes/Scope.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "Lexer/Token.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::LoopStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Create a metadata structure for this repeatable statement
    RepeatableStmnt & repeatableStmnt = mCtx.getRepeatableStmntForNode(astNode, astNode);
    
    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the 'loop' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    // Note: This is also the target of the 'next' statement.
    std::string startBBLbl = StringUtils::appendLineInfo("LoopStmntNoCond:block",
                                                         astNode.getStartToken());
    
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                          startBBLbl,
                                                          parentFn);
    
    WC_ASSERT(startBB);
    mCtx.mIRBuilder.CreateBr(startBB);
    repeatableStmnt.mNextStmntTargetBB = startBB;
    
    // Codegen the 'body' block:
    mCtx.mIRBuilder.SetInsertPoint(startBB);
    astNode.mBodyScope.accept(*this);
    
    // Create the end basic block: we go here on exiting the loop.
    // Note: this is also the target of the 'break' statement.
    std::string endBBLbl = StringUtils::appendLineInfo("LoopStmntNoCond:end",
                                                       astNode.getPastEndToken());
    
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                        endBBLbl,
                                                        parentFn);
    
    WC_ASSERT(endBB);
    repeatableStmnt.mBreakStmntTargetBB = endBB;
    
    // Create a branch back up to the top of the block for the body block.
    // This makes the loop happen. When done back to inserting code at the end block:
    mCtx.mIRBuilder.CreateBr(startBB);
    mCtx.mIRBuilder.SetInsertPoint(endBB);
    
    // Do any deferred codegen which needs to be done when the loop is finished
    mCtx.handleDeferredCodegenCallbacks(repeatableStmnt.mDeferredCodegenCallbacks);
}

void Codegen::visit(const AST::LoopStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Create a metadata structure for this repeatable statement
    RepeatableStmnt & repeatableStmnt = mCtx.getRepeatableStmntForNode(astNode, astNode);
    
    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the 'loop' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    std::string startBBLbl = StringUtils::appendLineInfo("LoopStmntWithCond:block",
                                                         astNode.getStartToken());
    
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                          startBBLbl,
                                                          parentFn);
    
    WC_ASSERT(startBB);
    mCtx.mIRBuilder.CreateBr(startBB);
    
    // Codegen the 'body' block:
    mCtx.mIRBuilder.SetInsertPoint(startBB);
    astNode.mBodyScope.accept(*this);
    
    // Create a block for the loop condition expression:
    // Note: also make the previous block branch to this block in order to properly terminate it.
    // Note: This is also the target of the 'next' statement.
    std::string loopCondBBLbl = StringUtils::appendLineInfo("LoopStmntWithCond:cond",
                                                            astNode.mLoopCondExpr.getStartToken());
    
    llvm::BasicBlock * loopCondBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                             loopCondBBLbl,
                                                             parentFn);
    
    WC_ASSERT(loopCondBB);
    mCtx.mIRBuilder.CreateBr(loopCondBB);
    repeatableStmnt.mNextStmntTargetBB = loopCondBB;
    
    // Generate the code for the loop condition expression:
    mCtx.mIRBuilder.SetInsertPoint(loopCondBB);
    astNode.mLoopCondExpr.accept(*this);
    Value loopExprVal = mCtx.popValue();
    
    // Generate the end basic block:
    // Note: This is also the target of the 'break' statement.
    std::string endBBLbl = StringUtils::appendLineInfo("LoopStmntWithCond:end",
                                                       astNode.getPastEndToken());
    
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                        endBBLbl,
                                                        parentFn);
    
    WC_ASSERT(endBB);
    repeatableStmnt.mBreakStmntTargetBB = endBB;
    
    // Expect the value to be a boolean:
    const DataType & loopExprType = loopExprVal.mCompiledType.getDataType();
    
    if (loopExprType.isBool()) {
        // Branch to the either the end or the start of the loop depending on the condition
        if (astNode.isLoopCondInversed()) {
            WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(loopExprVal.mLLVMVal, endBB, startBB));
        }
        else {
            WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(loopExprVal.mLLVMVal, startBB, endBB));
        }
    }
    else {
        // The loop condition must evaluate to a boolean
        mCtx.error(astNode.mLoopCondExpr,
                   "Condition expression for 'loop' statement must evaluate to type "
                   "'bool' not type '%s'!",
                   loopExprType.name().c_str());
        
        // Create an unreachable just to terminate the block
        mCtx.mIRBuilder.CreateUnreachable();
    }
    
    // Insert code after the end block from here on in
    mCtx.mIRBuilder.SetInsertPoint(endBB);
    
    // Do any deferred codegen which needs to be done when the loop is finished
    mCtx.handleDeferredCodegenCallbacks(repeatableStmnt.mDeferredCodegenCallbacks);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
