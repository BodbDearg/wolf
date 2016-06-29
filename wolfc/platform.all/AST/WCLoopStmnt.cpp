#include "WCLoopStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LoopStmnt
//-----------------------------------------------------------------------------

bool LoopStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kLoop;
}

LoopStmnt * LoopStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial 'loop' keyword
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "'loop' statement expected!");
        return nullptr;
    }
    
    // Skip the 'loop' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the body scope:
    Scope * bodyScope = Scope::parse(tokenPtr, alloc);
    WC_GUARD(bodyScope, nullptr);
    
    // Loop block should be terminated by an 'end' token:
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate 'loop' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = tokenPtr;
    ++tokenPtr;
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(alloc, LoopStmntNoCond, *bodyScope, *startToken, *endToken);
}

LoopStmnt::LoopStmnt(Scope & bodyScope, const Token & startToken) :
    mBodyScope(bodyScope),
    mStartToken(startToken)
{
    mBodyScope.mParent = this;
}

const Token & LoopStmnt::getStartToken() const {
    return mStartToken;
}

llvm::BasicBlock * LoopStmnt::getStartBlock() {
    return mStartBB;
}

llvm::BasicBlock * LoopStmnt::getEndBlock() {
    return mEndBB;
}

//-----------------------------------------------------------------------------
// LoopStmntNoCond
//-----------------------------------------------------------------------------

LoopStmntNoCond::LoopStmntNoCond(Scope & bodyScope, const Token & startToken, const Token & endToken) :
    LoopStmnt(bodyScope, startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & LoopStmntNoCond::getEndToken() const {
    return mEndToken;
}

bool LoopStmntNoCond::codegenStmnt(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the block for the loop statement:
    mStartBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntNoCond:block", parentFn);
    WC_ASSERT(mStartBB);
    
    // Make a branch to it to terminate the current block:
    cgCtx.irBuilder.CreateBr(mStartBB);

    // Codegen the 'body' block, this will go back up to the while condition block when done
    cgCtx.irBuilder.SetInsertPoint(mStartBB);
    
    if (!mBodyScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Create the end basic block:
    mEndBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntNoCond:end", parentFn);
    WC_ASSERT(mEndBB);
    
    // Create a branch back up to the top of the block for the current block
    cgCtx.irBuilder.CreateBr(mStartBB);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBB);
    return true;
}

WC_END_NAMESPACE
