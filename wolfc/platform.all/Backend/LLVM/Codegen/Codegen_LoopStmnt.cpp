#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../RepeatableStmnt.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/LoopStmnt.hpp"
#include "AST/Nodes/Scope.hpp"
#include "DataType/DataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::LoopStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a metadata structure for this repeatable statement
    RepeatableStmnt & repeatableStmnt = mCtx.getRepeatableStmntForNode(astNode, astNode);
    
    // Create the 'loop' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    // Note: This is also where the 'next' statement will branch to.
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                          "LoopStmntNoCond:block",
                                                          parentFn);
    
    WC_ASSERT(startBB);
    mCtx.mIRBuilder.CreateBr(startBB);
    repeatableStmnt.mNextStmntTargetBB = startBB;
    
    // Codegen the 'body' block:
    mCtx.mIRBuilder.SetInsertPoint(startBB);
    astNode.mBodyScope.accept(*this);
    
    // Create the end basic block: we go here on exiting the loop.
    // Note: this is also the target of the 'break' statement.
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                        "LoopStmntNoCond:end",
                                                        parentFn);
    
    WC_ASSERT(endBB);
    repeatableStmnt.mBreakStmntTargetBB = endBB;
    
    // Create a branch back up to the top of the block for the body block.
    // This makes the loop happen.
    // When done back to inserting code at the end block:
    mCtx.mIRBuilder.CreateBr(startBB);
    mCtx.mIRBuilder.SetInsertPoint(endBB);
    
    // Do any deferred codegen which needs to be done when the loop is finished
    mCtx.handleDeferredCodegenCallbacks(repeatableStmnt.mDeferredCodegenCallbacks);
}

void Codegen::visit(const AST::LoopStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
