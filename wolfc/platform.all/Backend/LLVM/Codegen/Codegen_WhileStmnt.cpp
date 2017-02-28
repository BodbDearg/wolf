#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../RepeatableStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/WhileStmnt.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "Lexer/Token.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::WhileStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Create a metadata structure for this repeatable statement
    RepeatableStmnt & repeatableStmnt = mCtx.getRepeatableStmntForNode(astNode, astNode);

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the 'while' basic block that does the condition check.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    // Note: this is also the target of 'next' statements
    std::string whileCondBBLbl = StringUtils::appendLineInfo("WhileStmnt:while_cond",
                                                             astNode.getStartToken());
    
    llvm::BasicBlock * whileCondBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                              whileCondBBLbl,
                                                              parentFn);
    
    WC_ASSERT(whileCondBB);
    mCtx.mIRBuilder.CreateBr(whileCondBB);
    repeatableStmnt.mNextStmntTargetBB = whileCondBB;
    
    // Generate the code for the while statement condition expression:
    mCtx.mIRBuilder.SetInsertPoint(whileCondBB);
    astNode.mWhileExpr.accept(*this);
    Value whileExprVal = mCtx.popValue();
    
    // Create the 'body' basic block:
    std::string bodyBBLbl = StringUtils::appendLineInfo("WhileStmnt:body",
                                                        astNode.mBodyScope.getStartToken());
    
    llvm::BasicBlock * bodyBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                         bodyBBLbl,
                                                         parentFn);
    
    WC_ASSERT(bodyBB);

    // Codegen the 'body' block, this will go back up to the while condition block when done
    mCtx.mIRBuilder.SetInsertPoint(bodyBB);
    astNode.mBodyScope.accept(*this);
    mCtx.mIRBuilder.CreateBr(whileCondBB);
    
    // Create the end basic block:
    // Note: this is also where the 'break' statement targets
    std::string endBBLbl = StringUtils::appendLineInfo("WhileStmnt:end",
                                                       astNode.mBodyScope.getPastEndToken());
    
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                        endBBLbl,
                                                        parentFn);
    
    WC_ASSERT(endBB);
    repeatableStmnt.mBreakStmntTargetBB = endBB;
    
    // Generate the branch for the while condition.
    // Note: the condition must be a boolean for this codegen to happen
    mCtx.mIRBuilder.SetInsertPoint(whileCondBB);
    const DataType & whileExprType = whileExprVal.mCompiledType.getDataType();
    
    if (whileExprType.isBool()) {
        if (astNode.isWhileExprInversed()) {
            WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(whileExprVal.mLLVMVal, endBB, bodyBB));
        }
        else {
            WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCondBr(whileExprVal.mLLVMVal, bodyBB, endBB));
        }
    }
    else {
        // The loop condition must evaluate to a boolean
        mCtx.error(astNode.mWhileExpr,
                   "Condition expression for 'while' loop must evaluate to type "
                   "'bool' not type '%s'!",
                   whileExprType.name().c_str());
        
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
