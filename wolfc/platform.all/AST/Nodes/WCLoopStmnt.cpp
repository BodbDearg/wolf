#include "WCLoopStmnt.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCScope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LoopStmnt
//-----------------------------------------------------------------------------
bool LoopStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kLoop;
}

LoopStmnt * LoopStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'loop' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("'loop' statement expected!");
        return nullptr;
    }
    
    // Skip the 'loop' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the body scope:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    
    // See if the current token is 'repeat', if it is then we have a loop/repeat statement
    if (parseCtx.tok()->type == TokenType::kRepeat) {
        // Loop block with a condition. Skip the 'repeat' token...
        parseCtx.nextTok();
        
        // Now grab the condition typpe Save the condition type (while/until) and skip this token..
        const Token * condTypeToken = parseCtx.tok();
        parseCtx.nextTok();
        
        if (condTypeToken->type != TokenType::kWhile && condTypeToken->type != TokenType::kUntil) {
            parseCtx.error("'while' or 'until' expected following 'repeat' token!");
            return nullptr;
        }
        
        // Now expect an assign expression as the condition
        AssignExpr * loopCondExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(loopCondExpr, nullptr);
        
        // All done, return the parsed expression:
        return WC_NEW_AST_NODE(parseCtx,
                               LoopStmntWithCond,
                               *bodyScope,
                               *startToken,
                               *condTypeToken,
                               *loopCondExpr);
    }
    else {
        // Conditionless loop block: should be terminated by an 'end' token:
        if (parseCtx.tok()->type != TokenType::kEnd) {
            parseCtx.error("'end' expected to terminate 'loop' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Done: return the parsed statement
        return WC_NEW_AST_NODE(parseCtx, LoopStmntNoCond, *bodyScope, *startToken, *endToken);
    }
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

bool LoopStmnt::allCodepathsHaveUncondRet() const {
    return mBodyScope.allCodepathsHaveUncondRet();
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

void LoopStmntNoCond::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

const Token & LoopStmntNoCond::getEndToken() const {
    return mEndToken;
}

#warning FIXME - Codegen
#if 0
llvm::BasicBlock * LoopStmntNoCond::getNextStmntTargetBlock() {
    return mStartBB;
}
    
llvm::BasicBlock * LoopStmntNoCond::getBreakStmntTargetBlock() {
    return mEndBB;
}

bool LoopStmntNoCond::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the 'loop' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    mStartBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntNoCond:block", parentFn);
    WC_ASSERT(mStartBB);
    cgCtx.irBuilder.CreateBr(mStartBB);

    // Codegen the 'body' block, this will go back up to the start of itself when done
    cgCtx.irBuilder.SetInsertPoint(mStartBB);
    
    if (!mBodyScope.codegen(cgCtx)) {
        return false;
    }
    
    // Create the end basic block: we go here on exiting the loop
    mEndBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntNoCond:end", parentFn);
    WC_ASSERT(mEndBB);
    
    // Create a branch back up to the top of the block for the body block
    cgCtx.irBuilder.CreateBr(mStartBB);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBB);
    return true;
}
#endif

//-----------------------------------------------------------------------------
// LoopStmntWithCond
//-----------------------------------------------------------------------------
LoopStmntWithCond::LoopStmntWithCond(Scope & bodyScope,
                                     const Token & startToken,
                                     const Token & condTypeToken,
                                     AssignExpr & loopCondExpr)
:
    LoopStmnt(bodyScope, startToken),
    mCondTypeToken(condTypeToken),
    mLoopCondExpr(loopCondExpr)
{
    WC_ASSERT(mCondTypeToken.type == TokenType::kWhile || mCondTypeToken.type == TokenType::kUntil);
    mLoopCondExpr.mParent = this;
}

void LoopStmntWithCond::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

const Token & LoopStmntWithCond::getEndToken() const {
    return mLoopCondExpr.getEndToken();
}

#warning FIXME - Codegen
#if 0
llvm::BasicBlock * LoopStmntWithCond::getNextStmntTargetBlock() {
    return mLoopCondBB;
}

llvm::BasicBlock * LoopStmntWithCond::getBreakStmntTargetBlock() {
    return mEndBB;
}

bool LoopStmntWithCond::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the 'loop' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntWithCond:block", parentFn);
    WC_ASSERT(startBB);
    cgCtx.irBuilder.CreateBr(startBB);
    
    // Codegen the 'body' block, this will go back up to the start of itself when done
    cgCtx.irBuilder.SetInsertPoint(startBB);
    
    if (!mBodyScope.codegen(cgCtx)) {
        return false;
    }
    
    // Create a block for the loop condition expression:
    // Note: also make the previous block branch to this block in order to properly terminate it.
    mLoopCondBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntWithCond:cond", parentFn);
    WC_ASSERT(mLoopCondBB);
    cgCtx.irBuilder.CreateBr(mLoopCondBB);
    
    // Generate the code for the loop condition expression:
    cgCtx.irBuilder.SetInsertPoint(mLoopCondBB);
    llvm::Value * loopCondResult = codegenLoopCondExpr(cgCtx);
    WC_GUARD(loopCondResult, false);
    
    // Generate the end basic block:
    mEndBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "LoopStmntWithCond:end", parentFn);
    WC_ASSERT(mEndBB);
    
    // Branch to the either the end or the start of the loop depending on the condition
    if (isLoopCondInversed()) {
        cgCtx.irBuilder.CreateCondBr(loopCondResult, mEndBB, startBB);
    }
    else {
        cgCtx.irBuilder.CreateCondBr(loopCondResult, startBB, mEndBB);
    }
    
    // Insert code after the end block from here on in
    cgCtx.irBuilder.SetInsertPoint(mEndBB);
    return true;
}
#endif

bool LoopStmntWithCond::isLoopCondInversed() const {
    return mCondTypeToken.type == TokenType::kUntil;
}

#warning FIXME - Codegen
#if 0
llvm::Value * LoopStmntWithCond::codegenLoopCondExpr(CodegenCtx & cgCtx) const {
    // Firstly validate that the loop condition expression is a bool:
    const DataType & condExprDataType = mLoopCondExpr.dataType();
    
    if (!condExprDataType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool))) {
        compileError("Condition for loop statement must evaluate to type 'bool', not '%s'!",
                     condExprDataType.name().c_str());
        
        return nullptr;
    }
    
    // Then generate the code
    return mLoopCondExpr.codegenExprEval(cgCtx);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
