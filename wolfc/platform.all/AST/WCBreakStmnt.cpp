#include "WCBreakStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCILoopStmnt.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool BreakStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kBreak;
}

BreakStmnt * BreakStmnt::parse(const Token *& tokenPtr) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected break statement!");
        return nullptr;
    }
    
    // Consume 'break' and return parsed statement
    const Token * breakTok = tokenPtr;
    ++tokenPtr;
    return new BreakStmnt(*breakTok);
}

BreakStmnt::BreakStmnt(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & BreakStmnt::getStartToken() const {
    return mToken;
}

const Token & BreakStmnt::getEndToken() const {
    return mToken;
}

bool BreakStmnt::codegenStmnt(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Grab the current insert block:
    llvm::BasicBlock * prevBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(prevBB);
    
    // Create the basic block for this statement
    mBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "BreakStmnt:stmnt", parentFn);
    WC_ASSERT(mBasicBlock);
    
    // Point the previous block to this new basic block:
    cgCtx.irBuilder.CreateBr(mBasicBlock);
    
    // Create a basic block for the code following the break and make it the new insert point.
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "BreakStmnt:end", parentFn);
    WC_ASSERT(endBB);
    cgCtx.irBuilder.SetInsertPoint(endBB);
    
    // Must defer the rest of the code generation until later
    cgCtx.deferredCodegenStmnts.push_back(this);
    return true;
}

bool BreakStmnt::deferredCodegenStmnt(CodegenCtx & cgCtx) {
    // Get the parent loop:
    ILoopStmnt * parentLoop = firstParentOfType<ILoopStmnt>();
    
    if (!parentLoop) {
        compileError("Break statement must have a parent loop! Break cannot be used outside of loops!");
        return false;
    }
    
    // Generate the jump to the end of the parent loop:
    cgCtx.irBuilder.SetInsertPoint(mBasicBlock);
    cgCtx.irBuilder.CreateBr(parentLoop->getEndBlock());
    return true;
}

WC_END_NAMESPACE
