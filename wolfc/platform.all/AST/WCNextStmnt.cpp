#include "WCNextStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIRepeatableStmnt.hpp"
#include "WCLinearAlloc.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NextStmnt
//-----------------------------------------------------------------------------

bool NextStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kNext;
}

NextStmnt * NextStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Check the basics
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected next statement!");
        return nullptr;
    }
    
    // Consume 'next' and save token for later:
    const Token * nextTok = tokenPtr;
    ++tokenPtr;
    
    // See whether 'if' or 'unless' follow, in which case the 'next' statement is conditional:
    if (tokenPtr->type == TokenType::kIf || tokenPtr->type == TokenType::kUnless) {
        // Parse the condition token:
        const Token * condTok = tokenPtr;
        ++tokenPtr;
        
        // Parse the condition assign expression:
        AssignExpr * condExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(condExpr, nullptr);
        
        // 'next' with a condition:
        return WC_NEW_AST_NODE(alloc, NextStmntWithCond, *nextTok, *condTok, *condExpr);
    }    
    
    // 'next' without a condition:
    return WC_NEW_AST_NODE(alloc, NextStmntNoCond, *nextTok);
}

NextStmnt::NextStmnt(const Token & nextToken) : mNextToken(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NextStmnt::getStartToken() const {
    return mNextToken;
}

bool NextStmnt::deferredCodegen(CodegenCtx & cgCtx) {
    // Get the parent repeatable statement:
    IRepeatableStmnt * parentRepeatableStmnt = firstParentOfType<IRepeatableStmnt>();
    
    if (!parentRepeatableStmnt) {
        compileError("'next' statement must have a parent repeatable block/loop! 'next' cannot be used outside of loops!");
        return false;
    }
    
    // Generate the jump to the next iteration of the parent loop:
    cgCtx.irBuilder.SetInsertPoint(mNextBlock);
    cgCtx.irBuilder.CreateBr(parentRepeatableStmnt->getNextStmntTargetBlock());
    return true;
}

//-----------------------------------------------------------------------------
// NextStmntNoCond
//-----------------------------------------------------------------------------

NextStmntNoCond::NextStmntNoCond(const Token & nextToken) : NextStmnt(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NextStmntNoCond::getEndToken() const {
    return mNextToken;
}

bool NextStmntNoCond::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the 'next' code
    mNextBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "NextStmntNoCond:stmnt", parentFn);
    WC_ASSERT(mNextBlock);
    
    // Point the previous block to this new basic block:
    cgCtx.irBuilder.CreateBr(mNextBlock);
    
    // Must defer the rest of the code generation until later
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegen(deferredCgCtx);
    });
    
    return true;    // All good so far!
}

//-----------------------------------------------------------------------------
// NextStmntWithCond
//-----------------------------------------------------------------------------

NextStmntWithCond::NextStmntWithCond(const Token & nextToken,
                                     const Token & condToken,
                                     AssignExpr & condExpr)
:
    NextStmnt(nextToken),
    mCondToken(condToken),
    mCondExpr(condExpr)
{
    mCondExpr.mParent = this;
}
    
const Token & NextStmntWithCond::getEndToken() const {
    return mCondExpr.getEndToken();
}
    
bool NextStmntWithCond::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the 'next' code
    mNextBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "NextStmntWithCond:next", parentFn);
    WC_ASSERT(mNextBlock);
    
    // Create the basic block for the continue code:
    mContinueBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "NextStmntWithCond:continue", parentFn);
    WC_ASSERT(mContinueBlock);
    
    // The assign expression must evaluate to bool:
    if (!mCondExpr.dataType().isBool()) {
        compileError("Condition for 'next' statement must evaluate to type 'bool', not '%s'!",
                     mCondExpr.dataType().name().c_str());
        
        return false;
    }
    
    // Generate the value for the condition assign expression:
    llvm::Value * condResult = mCondExpr.codegenExprEval(cgCtx);
    WC_GUARD(condResult, false);
    
    // Point the previous block to this new basic block:
    if (isIfCondInverted()) {
        cgCtx.irBuilder.CreateCondBr(condResult, mContinueBlock, mNextBlock);
    }
    else {
        cgCtx.irBuilder.CreateCondBr(condResult, mNextBlock, mContinueBlock);
    }
    
    // Future code should insert in the continue block:
    cgCtx.irBuilder.SetInsertPoint(mContinueBlock);
    
    // Must defer the rest of the code generation until later
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegen(deferredCgCtx);
    });
    
    return true;    // All good so far!
}

bool NextStmntWithCond::isIfCondInverted() const {
    return mCondToken.type == TokenType::kUnless;
}

WC_END_NAMESPACE
