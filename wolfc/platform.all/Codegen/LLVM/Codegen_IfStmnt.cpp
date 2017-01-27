#include "Codegen.hpp"

#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCIfStmnt.hpp"
#include "AST/Nodes/WCScope.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCStringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::IfStmntNoElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Generate the code for the if statement condition expression:
    const AST::AssignExpr & ifExpr = astNode.mIfExpr;
    ifExpr.accept(*this);
    Value ifExprVal = mCtx.popValue();
    
    // If the statement condition expression must be of type bool
    const DataType & ifExprDataType = ifExprVal.mCompiledType.getDataType();
    bool ifExprIsBool = ifExprDataType.isBool();
    
    if (!ifExprIsBool) {
        mCtx.error(ifExpr,
                   "Conditional expression for 'if' statement must evaluate to type 'bool' not '%s'!",
                   ifExprDataType.name().c_str());
    }
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = irb.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    const AST::Scope & thenScope = astNode.mThenScope;
    std::string thenBBLbl = StringUtils::appendLineInfo("IfStmntNoElse:then", thenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);

    // Codegen the 'then' scope
    irb.SetInsertPoint(thenBB);
    thenScope.accept(*this);
    
    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = irb.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = StringUtils::appendLineInfo("IfStmntNoElse:end", astNode.getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement.
    // Only do this if the conditions for doing this are valid however...
    irb.SetInsertPoint(ifBranchBB);
    
    if (ifExprVal.isValid() && ifExprIsBool) {
        if (astNode.isIfExprInversed()) {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, endBB, thenBB));
        }
        else {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, thenBB, endBB));
        }
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        irb.SetInsertPoint(thenEndBB);
        irb.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    irb.SetInsertPoint(endBB);
}

void Codegen::visit(const AST::IfStmntElseIf & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Generate the code for the if statement condition expression:
    const AST::AssignExpr & ifExpr = astNode.mIfExpr;
    ifExpr.accept(*this);
    Value ifExprVal = mCtx.popValue();
    
    // If the statement condition expression must be of type bool
    const DataType & ifExprDataType = ifExprVal.mCompiledType.getDataType();
    bool ifExprIsBool = ifExprDataType.isBool();
    
    if (!ifExprIsBool) {
        mCtx.error(ifExpr,
                   "Conditional expression for 'if' statement must evaluate to type 'bool' not '%s'!",
                   ifExprDataType.name().c_str());
    }
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = irb.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    const AST::Scope & thenScope = astNode.mThenScope;
    std::string thenBBLbl = StringUtils::appendLineInfo("IfStmntElseIf:then", thenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    // Codegen the 'then' scope
    irb.SetInsertPoint(thenBB);
    thenScope.accept(*this);
    
    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = irb.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the start basic block for the 'outer if' scope:
    const AST::IfStmnt & elseIfStmnt = astNode.mElseIfStmnt;
    std::string outerIfBBLbl = StringUtils::appendLineInfo("IfStmntElseIf:outer_if", elseIfStmnt.getStartToken());
    llvm::BasicBlock * outerIfBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, outerIfBBLbl, parentFn);
    WC_ASSERT(outerIfBB);
    
    // Codegen the 'outer if' scope
    irb.SetInsertPoint(outerIfBB);
    elseIfStmnt.accept(*this);
    
    // Get the current block, this is the end block for the 'outer if' scope:
    llvm::BasicBlock * outerIfEndBB = irb.GetInsertBlock();
    WC_ASSERT(outerIfEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = StringUtils::appendLineInfo("IfStmntElseIf:end", astNode.getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement.
    // Only do this if the conditions for doing this are valid however...
    irb.SetInsertPoint(ifBranchBB);
    
    if (ifExprVal.isValid() && ifExprIsBool) {
        if (astNode.isIfExprInversed()) {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, outerIfBB, thenBB));
        }
        else {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, thenBB, outerIfBB));
        }
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        irb.SetInsertPoint(thenEndBB);
        irb.CreateBr(endBB);
    }
    
    // Tie the end of the 'outer if' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!outerIfEndBB->getTerminator()) {
        irb.SetInsertPoint(outerIfEndBB);
        irb.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    irb.SetInsertPoint(endBB);
}

void Codegen::visit(const AST::IfStmntElse & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Generate the code for the if statement condition expression:
    const AST::AssignExpr & ifExpr = astNode.mIfExpr;
    ifExpr.accept(*this);
    Value ifExprVal = mCtx.popValue();
    
    // If the statement condition expression must be of type bool
    const DataType & ifExprDataType = ifExprVal.mCompiledType.getDataType();
    bool ifExprIsBool = ifExprDataType.isBool();
    
    if (!ifExprIsBool) {
        mCtx.error(ifExpr,
                   "Conditional expression for 'if' statement must evaluate to type 'bool' not '%s'!",
                   ifExprDataType.name().c_str());
    }
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = irb.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    const AST::Scope & thenScope = astNode.mThenScope;
    std::string thenBBLbl = StringUtils::appendLineInfo("IfStmntElse:then", thenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    // Codegen the 'then' scope
    irb.SetInsertPoint(thenBB);
    thenScope.accept(*this);

    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = irb.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the start basic block for the 'else' scope:
    const AST::Scope & elseScope = astNode.mElseScope;
    std::string elseBBLbl = StringUtils::appendLineInfo("IfStmntElse:else", elseScope.getStartToken());
    llvm::BasicBlock * elseBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, elseBBLbl, parentFn);
    WC_ASSERT(elseBB);
    
    // Codegen the 'else' scope
    irb.SetInsertPoint(elseBB);
    elseScope.accept(*this);
    
    // Get the current block, this is the end block for the 'else' scope:
    llvm::BasicBlock * elseEndBB = irb.GetInsertBlock();
    WC_ASSERT(elseEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = StringUtils::appendLineInfo("IfStmntElse:end", astNode.getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement.
    // Only do this if the conditions for doing this are valid however...
    irb.SetInsertPoint(ifBranchBB);
    
    if (ifExprVal.isValid() && ifExprIsBool) {
        if (astNode.isIfExprInversed()) {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, elseBB, thenBB));
        }
        else {
            WC_ASSERTED_OP(irb.CreateCondBr(ifExprVal.mLLVMVal, thenBB, elseBB));
        }
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        irb.SetInsertPoint(thenEndBB);
        irb.CreateBr(endBB);
    }
    
    // Tie the end of the 'else' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!elseEndBB->getTerminator()) {
        irb.SetInsertPoint(elseEndBB);
        irb.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    irb.SetInsertPoint(endBB);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
