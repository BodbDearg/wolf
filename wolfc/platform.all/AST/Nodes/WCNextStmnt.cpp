#include "WCNextStmnt.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCIRepeatableStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NextStmnt
//-----------------------------------------------------------------------------
bool NextStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kNext;
}

NextStmnt * NextStmnt::parse(ParseCtx & parseCtx) {
    // Check the basics
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected next statement!");
        return nullptr;
    }
    
    // Consume 'next' and save token for later:
    const Token * nextTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // See whether 'if' or 'unless' follow, in which case the 'next' statement is conditional:
    if (parseCtx.tok()->type == TokenType::kIf || parseCtx.tok()->type == TokenType::kUnless) {
        // Parse the condition token:
        const Token * condTok = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the condition assign expression:
        AssignExpr * condExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(condExpr, nullptr);
        
        // 'next' with a condition:
        return WC_NEW_AST_NODE(parseCtx, NextStmntWithCond, *nextTok, *condTok, *condExpr);
    }    
    
    // 'next' without a condition:
    return WC_NEW_AST_NODE(parseCtx, NextStmntNoCond, *nextTok);
}

NextStmnt::NextStmnt(const Token & nextToken) : mNextToken(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NextStmnt::getStartToken() const {
    return mNextToken;
}

#warning FIXME - Codegen
#if 0
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
#endif

//-----------------------------------------------------------------------------
// NextStmntNoCond
//-----------------------------------------------------------------------------
NextStmntNoCond::NextStmntNoCond(const Token & nextToken) : NextStmnt(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

void NextStmntNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NextStmntNoCond::getEndToken() const {
    return mNextToken;
}

#warning FIXME - Codegen
#if 0
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
#endif

bool NextStmntNoCond::allCodepathsHaveUncondRet() const {
    return true;
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

void NextStmntWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & NextStmntWithCond::getEndToken() const {
    return mCondExpr.getEndToken();
}

#warning FIXME - Codegen
#if 0
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
#endif

bool NextStmntWithCond::allCodepathsHaveUncondRet() const {
    return false;
}

bool NextStmntWithCond::isIfCondInverted() const {
    return mCondToken.type == TokenType::kUnless;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
