#include "WCNextStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCIRepeatableStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool NextStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kNext;
}

NextStmnt * NextStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected next statement!");
        return nullptr;
    }
    
    // Consume 'next' and return parsed statement
    const Token * breakTok = tokenPtr;
    ++tokenPtr;
    return WC_NEW_AST_NODE(alloc, NextStmnt, *breakTok);
}

NextStmnt::NextStmnt(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NextStmnt::getStartToken() const {
    return mToken;
}

const Token & NextStmnt::getEndToken() const {
    return mToken;
}

bool NextStmnt::codegenStmnt(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save current insert block:
    cgCtx.pushInsertBlock();    // TODO: still needed?
    
    // Create the basic block for this statement
    mBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "NextStmnt:stmnt", parentFn);
    WC_ASSERT(mBasicBlock);
    
    // Point the previous block to this new basic block:
    cgCtx.irBuilder.CreateBr(mBasicBlock);
    
    // Must defer the rest of the code generation until later
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegenStmnt(deferredCgCtx);
    });
    
    // Restore previous block:
    cgCtx.popInsertBlock();     // TODO: still needed?
    return true;
}

bool NextStmnt::deferredCodegenStmnt(CodegenCtx & cgCtx) {
    // Get the parent repeatable statement:
    IRepeatableStmnt * parentRepeatableStmnt = firstParentOfType<IRepeatableStmnt>();
    
    if (!parentRepeatableStmnt) {
        compileError("'next' statement must have a parent repeatable block/loop! 'next' cannot be used outside of loops!");
        return false;
    }
    
    // Generate the jump to the next iteration of the parent loop:
    cgCtx.irBuilder.SetInsertPoint(mBasicBlock);
    cgCtx.irBuilder.CreateBr(parentRepeatableStmnt->getNextStmntTargetBlock());
    return true;
}

WC_END_NAMESPACE
