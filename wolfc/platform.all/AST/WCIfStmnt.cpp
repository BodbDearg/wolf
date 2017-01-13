#include "WCIfStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCScope.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// IfExpr
//-----------------------------------------------------------------------------
bool IfStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kIf || tokenType == TokenType::kUnless;
}

IfStmnt * IfStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'if' or 'unless' keyword
    if (!peek(parseCtx.curTok)) {
        parseError(parseCtx, "If statement expected!");
        return nullptr;
    }
    
    // Skip the 'if' or 'unless' token and save location
    const Token * startToken = parseCtx.curTok;
    parseCtx.nextTok();
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(ifExpr, nullptr);
    
    // See if there is a 'then' following. This keyword is optional, unless the 'then' scope is required
    // to be on the same line as the enclosing if statement:
    bool thenScopeRequiresNL = true;
    
    if (parseCtx.curTok->type == TokenType::kThen) {
        // Found a 'then' token, skip it. The 'then' scope is allowed to be on the same line
        parseCtx.nextTok();
        thenScopeRequiresNL = false;
    }
    
    // Expect scope following:
    Scope * thenScope = Scope::parse(parseCtx);
    WC_GUARD(thenScope, nullptr);
    
    // See if it violates newline rules:
    if (thenScopeRequiresNL) {
        if (thenScope->getStartToken().startLine == ifExpr->getEndToken().endLine) {
            parseError(parseCtx,
                       thenScope->getStartToken(),
                       "Code following 'if' statement condition must be on a new line unless "
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
    if (parseCtx.curTok->type == TokenType::kElse) {
        // (3) if statement with an else, skip the 'else' token.
        parseCtx.nextTok();
        
        // Parse the scope for the 'else' block:
        Scope * elseScope = Scope::parse(parseCtx);
        WC_GUARD(elseScope, nullptr);
        
        // Else block should be terminated by an 'end' token:
        if (parseCtx.curTok->type != TokenType::kEnd) {
            parseError(parseCtx, "'end' expected to terminate 'else' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = parseCtx.curTok;
        parseCtx.nextTok();
        
        // Done, return the parsed statement:
        return WC_NEW_AST_NODE(parseCtx,
                               IfStmntElse,
                               *ifExpr,
                               *thenScope,
                               *elseScope,
                               *startToken,
                               *endToken);
    }
    else if (parseCtx.curTok->type == TokenType::kOr) {
        // (2) if statement with an 'or if' chained if statement, skip the 'or' token.
        parseCtx.nextTok();
        
        // Parse the if statement following the 'or':
        IfStmnt * outerIfStmnt = IfStmnt::parse(parseCtx);
        WC_GUARD(outerIfStmnt, nullptr);
        
        // Done, return the parsed statement:
        return WC_NEW_AST_NODE(parseCtx,
                               IfStmntElseIf,
                               *ifExpr,
                               *thenScope,
                               *outerIfStmnt,
                               *startToken);
    }
    else {
        // (1) 'if then' type statement: expect closing 'end'
        if (parseCtx.curTok->type != TokenType::kEnd) {
            parseError(parseCtx, "'end' expected to terminate 'if' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = parseCtx.curTok;
        parseCtx.nextTok();
        
        // Done, return the parsed statement
        return WC_NEW_AST_NODE(parseCtx,
                               IfStmntNoElse,
                               *ifExpr,
                               *thenScope,
                               *startToken,
                               *endToken);
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

llvm::Value * IfStmnt::codegenIfExpr(CodegenCtx & cgCtx) const {
    // Firstly validate that the if statement condition expression is a bool;
    const DataType & ifExprDataType = mIfExpr.dataType();
    
    if (!ifExprDataType.isBool()) {
        compileError("Condition for if statement must evaluate to type 'bool', not '%s'!",
                     ifExprDataType.name().c_str());
        
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

bool IfStmntNoElse::codegen(CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntNoElse:then", mThenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);

    // Codegen the 'then' scope
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    WC_GUARD(mThenScope.codegen(cgCtx), false);
    
    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = makeLLVMLabelForTok("IfStmntNoElse:end", getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement
    cgCtx.irBuilder.SetInsertPoint(ifBranchBB);
    
    if (isIfExprInversed()) {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, endBB, thenBB));
    }
    else {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, endBB));
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        cgCtx.irBuilder.SetInsertPoint(thenEndBB);
        cgCtx.irBuilder.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    cgCtx.irBuilder.SetInsertPoint(endBB);
    return true;
}

bool IfStmntNoElse::allCodepathsHaveUncondRet() const {
    return false;
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

bool IfStmntElseIf::codegen(CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntElseIf:then", mThenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    // Codegen the 'then' scope
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    WC_GUARD(mThenScope.codegen(cgCtx), false);
    
    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the start basic block for the 'outer if' scope:
    std::string outerIfBBLbl = makeLLVMLabelForTok("IfStmntElseIf:outer_if", mElseIfStmnt.getStartToken());
    llvm::BasicBlock * outerIfBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, outerIfBBLbl, parentFn);
    WC_ASSERT(outerIfBB);
    
    // Codegen the 'outer if' scope
    cgCtx.irBuilder.SetInsertPoint(outerIfBB);
    WC_GUARD(mElseIfStmnt.codegen(cgCtx), false);
    
    // Get the current block, this is the end block for the 'outer if' scope:
    llvm::BasicBlock * outerIfEndBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(outerIfEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = makeLLVMLabelForTok("IfStmntElseIf:end", getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement
    cgCtx.irBuilder.SetInsertPoint(ifBranchBB);
    
    if (isIfExprInversed()) {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, outerIfBB, thenBB));
    }
    else {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, outerIfBB));
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        cgCtx.irBuilder.SetInsertPoint(thenEndBB);
        cgCtx.irBuilder.CreateBr(endBB);
    }
    
    // Tie the end of the 'outer if' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!outerIfEndBB->getTerminator()) {
        cgCtx.irBuilder.SetInsertPoint(outerIfEndBB);
        cgCtx.irBuilder.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    cgCtx.irBuilder.SetInsertPoint(endBB);
    return true;
}

bool IfStmntElseIf::allCodepathsHaveUncondRet() const {
    return  mThenScope.allCodepathsHaveUncondRet() &&
            mElseIfStmnt.allCodepathsHaveUncondRet();
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
    
bool IfStmntElse::codegen(CodegenCtx & cgCtx) {
    // Generate the code for the if statement condition expression:
    llvm::Value * ifExprResult = codegenIfExpr(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save basic block that the 'if' branch will go into:
    llvm::BasicBlock * ifBranchBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBranchBB);
    
    // Create the start basic block for the 'then' scope:
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntElse:then", mThenScope.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    // Codegen the 'then' scope
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    WC_GUARD(mThenScope.codegen(cgCtx), false);

    // Get the current block, this is the end block for the 'then' scope:
    llvm::BasicBlock * thenEndBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(thenEndBB);
    
    // Create the start basic block for the 'else' scope:
    std::string elseBBLbl = makeLLVMLabelForTok("IfStmntElse:else", mElseScope.getStartToken());
    llvm::BasicBlock * elseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, elseBBLbl, parentFn);
    WC_ASSERT(elseBB);
    
    // Codegen the 'else' scope
    cgCtx.irBuilder.SetInsertPoint(elseBB);
    WC_GUARD(mElseScope.codegen(cgCtx), false);
    
    // Get the current block, this is the end block for the 'else' scope:
    llvm::BasicBlock * elseEndBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(elseEndBB);
    
    // Create the end basic block for the if statement:
    std::string endBBLbl = makeLLVMLabelForTok("IfStmntElse:end", getEndToken());
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, endBBLbl, parentFn);
    WC_ASSERT(endBB);
    
    // Generate the branch for the if statement
    cgCtx.irBuilder.SetInsertPoint(ifBranchBB);
    
    if (isIfExprInversed()) {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, elseBB, thenBB));
    }
    else {
        WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, elseBB));
    }
    
    // Tie the end of the 'then' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!thenEndBB->getTerminator()) {
        cgCtx.irBuilder.SetInsertPoint(thenEndBB);
        cgCtx.irBuilder.CreateBr(endBB);
    }
    
    // Tie the end of the 'else' scope to the end of this if statement,
    // that is if it is not already terminated:
    if (!elseEndBB->getTerminator()) {
        cgCtx.irBuilder.SetInsertPoint(elseEndBB);
        cgCtx.irBuilder.CreateBr(endBB);
    }
    
    // Insert future code past the end of this if statement
    cgCtx.irBuilder.SetInsertPoint(endBB);
    return true;
}

bool IfStmntElse::allCodepathsHaveUncondRet() const {
    return  mThenScope.allCodepathsHaveUncondRet() &&
            mElseScope.allCodepathsHaveUncondRet();
}

WC_END_NAMESPACE
