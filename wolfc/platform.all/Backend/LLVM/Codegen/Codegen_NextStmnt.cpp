//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "../RepeatableStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/NextStmnt.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * Codegen the logic which does the 'next' statement. 
 * This codegen is deferred until a while later. 
 */
static void deferredCodegenNextLogic(CodegenCtx & ctx,
                                     const AST::NextStmnt & astNode,
                                     llvm::BasicBlock & nextBB)
{
    // In order to generate the 'next' logic we first need to get the parent repeatable statement.
    RepeatableStmnt * repeatableStmnt = ctx.getCurrentRepeatableStmnt();
    
    if (!repeatableStmnt) {
        ctx.error(astNode,
                  "A 'next' statement must occur within a loop of some sort, such as "
                  "a 'loop' statement, 'while' statement etc.");
        
        return;
    }
    
    // The deferred logic to create the jump for the 'next' statement
    repeatableStmnt->mDeferredCodegenCallbacks.push_back([&ctx,
                                                          &astNode,
                                                          &nextBB,
                                                          repeatableStmnt]()
    {
        // Expect the repeatable statement to have a 'next' statement target at this point
        if (!repeatableStmnt->mNextStmntTargetBB) {
            ctx.error(astNode,
                      "Internal error! Failed to codegen the 'next' statement because the parent "
                      "repeatable statement does not define a jump target for the 'next' statement!");
            
            return;
        }
    
        // Generate the jump to wherever 'next' should land us.
        // Save and restore the current code block also so as not to disturb other code generation.
        ctx.pushInsertBlock();
        ctx.mIRBuilder.SetInsertPoint(&nextBB);
        ctx.mIRBuilder.CreateBr(repeatableStmnt->mNextStmntTargetBB);
        ctx.popInsertBlock();
    });
}

void Codegen::visit(const AST::NextStmntNoCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the 'next' code
    std::string nextBBLbl = StringUtils::appendLineInfo("NextStmntNoCond:stmnt", astNode.getStartToken());
    llvm::BasicBlock * nextBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, nextBBLbl, parentFn);
    WC_ASSERT(nextBB);
    
    // Point the previous block to this new basic block:
    mCtx.mIRBuilder.CreateBr(nextBB);
    
    // Schedule the deferred codegen for the next logic
    deferredCodegenNextLogic(mCtx, astNode, *nextBB);
}

void Codegen::visit(const AST::NextStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the next code
    std::string nextBBLbl = StringUtils::appendLineInfo("NextStmntWithCond:next", astNode.getStartToken());
    llvm::BasicBlock * nextBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, nextBBLbl, parentFn);
    WC_ASSERT(nextBB);
    
    // Create the basic block for the continue code:
    std::string continueBBLbl = StringUtils::appendLineInfo("NextStmntWithCond:continue", astNode.getPastEndToken());
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, continueBBLbl, parentFn);
    WC_ASSERT(continueBB);
    
    // Evaluate the expression for the next condition
    astNode.mCondExpr.accept(*this);
    Value condExprVal = mCtx.popValue();
    
    // Compile the bool data type and implicitly cast to it if we can
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    ImplicitCasts::castSingleValueIfRequired(*this, condExprVal, boolCDT);
    
    // The assign expression must evaluate to bool:
    const DataType & condExprType = condExprVal.mCompiledType.getDataType();
    
    if (!condExprType.isBool()) {
        // Note: don't issue an error in the case of 'undefined' since this means we've already done that for this expr
        if (!condExprType.isUndefined()) {
            mCtx.error("Condition for 'next' statement must evaluate to type 'bool', not '%s'!",
                       condExprType.name().c_str());
        }
        
        return;
    }
    
    // Depending on the result of the condition expression, either jump to the continue block or the break block.
    WC_ASSERT(condExprVal.mLLVMVal);
    
    if (astNode.isIfCondInverted()) {
        mCtx.mIRBuilder.CreateCondBr(condExprVal.mLLVMVal, continueBB, nextBB);
    }
    else {
        mCtx.mIRBuilder.CreateCondBr(condExprVal.mLLVMVal, nextBB, continueBB);
    }
    
    // Future code should insert in the continue block:
    mCtx.mIRBuilder.SetInsertPoint(continueBB);
    
    // Schedule the deferred codegen for the next logic
    deferredCodegenNextLogic(mCtx, astNode, *nextBB);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
