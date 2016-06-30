#include "WCScopeStmnt.hpp"
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
// ScopeStmnt
//-----------------------------------------------------------------------------

bool ScopeStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kScope;
}

ScopeStmnt * ScopeStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial 'scope' keyword
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "'scope' statement expected!");
        return nullptr;
    }
    
    // Skip the 'scope' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the body scope:
    Scope * bodyScope = Scope::parse(tokenPtr, alloc);
    WC_GUARD(bodyScope, nullptr);
    
    // Must be terminated by an 'end' token
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate 'scope' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = tokenPtr;
    ++tokenPtr;
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(alloc, ScopeStmnt, *startToken, *bodyScope, *endToken);
}

ScopeStmnt::ScopeStmnt(const Token & startToken, Scope & bodyScope, const Token & endToken) :
    mStartToken(startToken),
    mBodyScope(bodyScope),
    mEndToken(endToken)
{
    mBodyScope.mParent = this;
}

const Token & ScopeStmnt::getStartToken() const {
    return mStartToken;
}

const Token & ScopeStmnt::getEndToken() const {
    return mEndToken;
}

bool ScopeStmnt::codegenStmnt(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the 'scope' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ScopeStmnt:block", parentFn);
    WC_ASSERT(startBB);
    cgCtx.irBuilder.CreateBr(startBB);

    // Codegen the main body scope:
    cgCtx.irBuilder.SetInsertPoint(startBB);
    
    if (!mBodyScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Create the end basic block and make the current block branch to it to terminate it:
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ScopeStmnt:end", parentFn);
    WC_ASSERT(endBB);
    cgCtx.irBuilder.CreateBr(endBB);
    
    // Switch to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(endBB);
    return true;
}

WC_END_NAMESPACE
