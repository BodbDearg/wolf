//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/ScopeStmnt.hpp"
#include "Assert.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::ScopeStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // There should be a parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the 'scope' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    std::string startBBLabel = StringUtils::appendLineInfo("ScopeStmnt:block",
                                                           astNode.getStartToken());
    
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, startBBLabel, parentFn);
    WC_ASSERT(startBB);
    mCtx.mIRBuilder.CreateBr(startBB);

    // Codegen the main body scope:
    mCtx.mIRBuilder.SetInsertPoint(startBB);
    astNode.mBodyScope.accept(*this);
    
    // Create the end basic block and make the current block branch to it to terminate it:
    std::string endBBLabel = StringUtils::appendLineInfo("ScopeStmnt:end",
                                                         astNode.getEndToken());
    
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLabel, parentFn);
    WC_ASSERT(endBB);
    mCtx.mIRBuilder.CreateBr(endBB);
    
    // Switch to inserting code at the end block:
    mCtx.mIRBuilder.SetInsertPoint(endBB);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
