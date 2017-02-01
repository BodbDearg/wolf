#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../RepeatableStmnt.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/BreakStmnt.hpp"
#include "DataType/DataType.hpp"
#include "Lexer/Token.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::BreakStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the 'break' code
    std::string breakBBLbl = StringUtils::appendLineInfo("BreakStmntNoCond:break", astNode.getStartToken());
    llvm::BasicBlock * breakBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, breakBBLbl, parentFn);
    WC_ASSERT(breakBB);
    
    // Point the previous block to this new basic block:
    mCtx.mIRBuilder.CreateBr(breakBB);
    
    // Must defer the rest of the code generation until later, grab the current repeatable statement:
    RepeatableStmnt * repeatableStmnt = mCtx.getCurrentRepeatableStmnt();
    
    if (repeatableStmnt) {
        repeatableStmnt->mDeferredCodegenCallbacks.push_back([=](){
            // Expect the repeatable statement to have a break statement target at this point
            if (repeatableStmnt->mBreakStmntTargetBB) {
                // Generate the jump to past the end of the parent loop:
                mCtx.pushInsertBlock();
                mCtx.mIRBuilder.SetInsertPoint(breakBB);
                mCtx.mIRBuilder.CreateBr(repeatableStmnt->mBreakStmntTargetBB);
                mCtx.popInsertBlock();
            }
            else {
                mCtx.error(astNode,
                           "Internal error! Failed to codegen the 'break' statement because the parent "
                           "repeatable statement does not define a jump target for the break statement!");
            }
        });
    }
    else {
        mCtx.error(astNode,
                   "A break statement must occur within a loop of some sort, such as "
                   "a 'loop' statement, 'while' statement etc.");
    }
}

void Codegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
