#include "WCIfStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// IfExpr
//-----------------------------------------------------------------------------

bool IfStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kIf || tokenType == TokenType::kUnless;
}

IfStmnt * IfStmnt::parse(const Token *& tokenPtr) {
    // Parse the initial 'if' or 'unless' keyword
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "If statement expected!");
        return nullptr;
    }
    
    // Skip the 'if' or 'unless' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(ifExpr, nullptr);
    
    // See if there is a 'then' following. This keyword is optional, unless the 'then' scope is required
    // to be on the same line as the enclosing if statement:
    bool thenScopeRequiresNL = true;
    
    if (tokenPtr->type == TokenType::kThen) {
        // Found a 'then' token, skip it. The 'then' scope is allowed to be on the same line
        ++tokenPtr;
        thenScopeRequiresNL = false;
    }
    
    // Expect scope following:
    Scope * thenScope = Scope::parse(tokenPtr);
    WC_GUARD(thenScope, nullptr);
    
    // See if it violates newline rules:
    if (thenScopeRequiresNL) {
        if (thenScope->getStartToken().startLine == ifExpr->getEndToken().endLine) {
            parseError(thenScope->getStartToken(), "Code following 'if' statement condition must be on a new line unless "
                       "'then' is used after the condition.");
            
            return nullptr;
        }
    }
    
    // 3 possibilities can follow:
    //
    // 1 - 'end', for a simple 'if' statement without any 'or if' or 'else'.
    // 2 - 'or if' for an chained 'elseif' type statement
    // 3 - 'else' for an 'if' statement with an else block
    //
    if (tokenPtr->type == TokenType::kElse) {
        // (3) if statement with an else, skip the 'else' token.
        ++tokenPtr;
        
        // Parse the scope for the 'else' block:
        Scope * elseScope = Scope::parse(tokenPtr);
        WC_GUARD(elseScope, nullptr);
        
        // Else block should be terminated by an 'end' token:
        if (tokenPtr->type != TokenType::kEnd) {
            parseError(*tokenPtr, "'end' expected to terminate 'else' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = tokenPtr;
        ++tokenPtr;
        
        // Done, return the parsed statement:
        return new IfStmntElse(*ifExpr, *thenScope, *elseScope, *startToken, *endToken);
    }
    else if (tokenPtr->type == TokenType::kOr) {
        // (2) if statement with an 'or if' chained if statement, skip the 'or' token.
        ++tokenPtr;
        
        // Parse the if statement following the 'or':
        IfStmnt * outerIfStmnt = IfStmnt::parse(tokenPtr);
        WC_GUARD(outerIfStmnt, nullptr);
        
        // Done, return the parsed statement:
        return new IfStmntElseIf(*ifExpr, *thenScope, *outerIfStmnt, *startToken);
    }
    else {
        // (1) 'if then' type statement: expect closing 'end'
        if (tokenPtr->type != TokenType::kEnd) {
            parseError(*tokenPtr, "'end' expected to terminate 'if' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = tokenPtr;
        ++tokenPtr;
        
        // Done, return the parsed statement
        return new IfStmntNoElse(*ifExpr, *thenScope, *startToken, *endToken);
    }
}

IfStmnt::IfStmnt(AssignExpr & ifExpr,
                 Scope & thenScope,
                 const Token & startToken)
:
    mIfExpr(ifExpr),
    mThenScope(thenScope),
    mStartToken(startToken)
{
    mIfExpr.mParent = this;
    mThenScope.mParent = this;
}

const Token & IfStmnt::getStartToken() const {
    return mStartToken;
}

bool IfStmnt::isIfExprInversed() const {
    return mStartToken.type == TokenType::kUnless;
}

llvm::Value * IfStmnt::codegenIfExpr(const CodegenCtx & cgCtx) const {
    // Firstly validate that the if statement condition expression is a bool;
    const DataType & ifExprDataType = mIfExpr.dataType();
    
    if (!ifExprDataType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool))) {
        compileError("Condition for if statement must evaluate to type 'bool', not '%s'!",
                     ifExprDataType.name());
        
        return nullptr;
    }
    
    // Then generate the code
    return mIfExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// IfStmntNoElse
//-----------------------------------------------------------------------------

IfStmntNoElse::IfStmntNoElse(AssignExpr & ifExpr,
                             Scope & thenScope,
                             const Token & startToken,
                             const Token & endToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & IfStmntNoElse::getEndToken() const {
    return mEndToken;
}

bool IfStmntNoElse::codegenStmnt(const CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBB);
    
    // Create the 'then' and 'end' basic blocks:
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntNoElse:then", parentFn);
    WC_ASSERT(thenBB);
    mEndBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntNoElse:end", parentFn);
    WC_ASSERT(mEndBasicBlock);

    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(mEndBasicBlock);
    
    // Generate the branch for the if
    cgCtx.irBuilder.SetInsertPoint(ifBB);
    llvm::Value * branch = nullptr;
    
    if (isIfExprInversed()) {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, mEndBasicBlock, thenBB);
    }
    else {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, mEndBasicBlock);
    }
    
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    return true;
}

//-----------------------------------------------------------------------------
// IfStmntElseIf
//-----------------------------------------------------------------------------

IfStmntElseIf::IfStmntElseIf(AssignExpr & ifExpr,
                             Scope & thenScope,
                             IfStmnt & outerIfStmnt,
                             const Token & startToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mElseIfStmnt(outerIfStmnt)
{
    mElseIfStmnt.mParent = this;
}

const Token & IfStmntElseIf::getEndToken() const {
    return mElseIfStmnt.getEndToken();
}

bool IfStmntElseIf::codegenStmnt(const CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBB);
    
    // Create the 'then' and 'outer if' blocks:
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElseIf:then", parentFn);
    WC_ASSERT(thenBB);
    llvm::BasicBlock * outerIfBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElseIf:outer_if", parentFn);
    WC_ASSERT(outerIfBB);
    
    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Codegen the 'outer if' block
    cgCtx.irBuilder.SetInsertPoint(outerIfBB);
    
    if (!mElseIfStmnt.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Expect the outer if to have an 'end' basic block. Tie the end of this if's 'then' block to that:
    WC_ASSERT(mElseIfStmnt.mEndBasicBlock);
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    mEndBasicBlock = mElseIfStmnt.mEndBasicBlock;
    cgCtx.irBuilder.CreateBr(mEndBasicBlock);
    
    // Generate the branch for the if
    cgCtx.irBuilder.SetInsertPoint(ifBB);
    llvm::Value * branch = nullptr;
    
    if (isIfExprInversed()) {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, outerIfBB, thenBB);
    }
    else {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, outerIfBB);
    }
    
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    return true;
}

//-----------------------------------------------------------------------------
// IfStmntElse
//-----------------------------------------------------------------------------

IfStmntElse::IfStmntElse(AssignExpr & ifExpr,
                         Scope & thenScope,
                         Scope & elseScope,
                         const Token & startToken,
                         const Token & endToken)
:
    IfStmnt(ifExpr, thenScope, startToken),
    mElseScope(elseScope),
    mEndToken(endToken)
{
    mElseScope.mParent = this;
}
    
const Token & IfStmntElse::getEndToken() const {
    return mEndToken;
}
    
bool IfStmntElse::codegenStmnt(const CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    
    // Create the 'then', 'else' and 'end' blocks:
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElse:then", parentFn);
    WC_ASSERT(thenBB);
    llvm::BasicBlock * elseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElse:else", parentFn);
    WC_ASSERT(elseBB);
    mEndBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElse:end", parentFn);
    WC_ASSERT(mEndBasicBlock);
    
    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(mEndBasicBlock);
    
    // Codegen the 'else' block
    cgCtx.irBuilder.SetInsertPoint(elseBB);
    
    if (!mElseScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(mEndBasicBlock);
    
    // Generate the branch for the if
    cgCtx.irBuilder.SetInsertPoint(ifBB);
    llvm::Value * branch = nullptr;
    
    if (isIfExprInversed()) {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, elseBB, thenBB);
    }
    else {
        branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, elseBB);
    }
    
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    return true;
}

WC_END_NAMESPACE
