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

/* Codegen the logic which does the break. This codegen is deferred until a while later. */
static void deferredCodegenBreakLogic(CodegenCtx & ctx,
                                      const AST::BreakStmnt & astNode,
                                      llvm::BasicBlock & breakBB)
{
    // In order to generate the break logic we first need to get the parent repeatable statement.
    RepeatableStmnt * repeatableStmnt = ctx.getCurrentRepeatableStmnt();
    
    if (!repeatableStmnt) {
        ctx.error(astNode,
                  "A break statement must occur within a loop of some sort, such as "
                  "a 'loop' statement, 'while' statement etc.");
        
        return;
    }
    
    // The deferred logic to create the jump for the break statement
    repeatableStmnt->mDeferredCodegenCallbacks.push_back([&ctx,
                                                          &astNode,
                                                          &breakBB,
                                                          repeatableStmnt]()
    {
        // Expect the repeatable statement to have a break statement target at this point
        if (!repeatableStmnt->mBreakStmntTargetBB) {
            ctx.error(astNode,
                      "Internal error! Failed to codegen the 'break' statement because the parent "
                      "repeatable statement does not define a jump target for the break statement!");
            
            return;
        }
    
        // Generate the jump to past the end of the parent loop:
        ctx.pushInsertBlock();
        ctx.mIRBuilder.SetInsertPoint(&breakBB);
        ctx.mIRBuilder.CreateBr(repeatableStmnt->mBreakStmntTargetBB);
        ctx.popInsertBlock();
    });
}

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
    
    // Schedule the deferred codegen for the break logic
    deferredCodegenBreakLogic(mCtx, astNode, *breakBB);
}

void Codegen::visit(const AST::BreakStmntWithCond & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the break code
    std::string breakBBLbl = StringUtils::appendLineInfo("BreakStmntWithCond:break", astNode.getStartToken());
    llvm::BasicBlock * breakBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, breakBBLbl, parentFn);
    WC_ASSERT(breakBB);
    
    // Create the basic block for the continue code:
    std::string continueBBLbl = StringUtils::appendLineInfo("BreakStmntWithCond:continue", astNode.getStartToken());
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, continueBBLbl, parentFn);
    WC_ASSERT(continueBB);
    
    // Evaluate the expression for the break condition
    astNode.mCondExpr.accept(*this);
    Value condExprVal = mCtx.popValue();
    const DataType & condExprType = condExprVal.mCompiledType.getDataType();
    
    // The assign expression must evaluate to bool:
    if (!condExprType.isBool()) {
        mCtx.error("Condition for 'break' statement must evaluate to type 'bool', not '%s'!",
                   condExprType.name().c_str());
        
        return;
    }
    
    // Depending on the result of the condition expression, either jump to the
    // continue block or the break block.
    if (astNode.isIfCondInverted()) {
        mCtx.mIRBuilder.CreateCondBr(condExprVal.mLLVMVal, continueBB, breakBB);
    }
    else {
        mCtx.mIRBuilder.CreateCondBr(condExprVal.mLLVMVal, breakBB, continueBB);
    }
    
    // Future code should insert in the continue block:
    mCtx.mIRBuilder.SetInsertPoint(continueBB);
    
    // Schedule the deferred codegen for the break logic
    deferredCodegenBreakLogic(mCtx, astNode, *breakBB);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
