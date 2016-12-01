#include "WCIfStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCScope.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// IfExpr
//-----------------------------------------------------------------------------

bool IfStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kIf || tokenType == TokenType::kUnless;
}

IfStmnt * IfStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial 'if' or 'unless' keyword
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "If statement expected!");
        return nullptr;
    }
    
    // Skip the 'if' or 'unless' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(tokenPtr, alloc);
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
    Scope * thenScope = Scope::parse(tokenPtr, alloc);
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
        Scope * elseScope = Scope::parse(tokenPtr, alloc);
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
        return WC_NEW_AST_NODE(alloc, IfStmntElse, *ifExpr, *thenScope, *elseScope, *startToken, *endToken);
    }
    else if (tokenPtr->type == TokenType::kOr) {
        // (2) if statement with an 'or if' chained if statement, skip the 'or' token.
        ++tokenPtr;
        
        // Parse the if statement following the 'or':
        IfStmnt * outerIfStmnt = IfStmnt::parse(tokenPtr, alloc);
        WC_GUARD(outerIfStmnt, nullptr);
        
        // Done, return the parsed statement:
        return WC_NEW_AST_NODE(alloc, IfStmntElseIf, *ifExpr, *thenScope, *outerIfStmnt, *startToken);
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
        return WC_NEW_AST_NODE(alloc, IfStmntNoElse, *ifExpr, *thenScope, *startToken, *endToken);
    }
}

IfStmnt::IfStmnt(AssignExpr & ifExpr,
                 IBasicCodegenNode & thenNode,
                 const Token & startToken)
:
    mIfExpr(ifExpr),
    mThenNode(thenNode),
    mStartToken(startToken)
{
    mIfExpr.mParent = this;
    
    // Need to do a tricky cast for this:
    {
        ASTNode * thenASTNode = dynamic_cast<ASTNode*>(&thenNode);
        WC_ASSERT(thenASTNode);
        thenASTNode->mParent = this;
    }
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
                             IBasicCodegenNode & thenNode,
                             const Token & startToken,
                             const Token & endToken)
:
    IfStmnt(ifExpr, thenNode, startToken),
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
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBB);
    
    // Create the 'then' and 'end' basic blocks:
    ASTNode & thenASTNode = dynamic_cast<ASTNode&>(mThenNode);
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntNoElse:then", thenASTNode.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    std::string endBBLbl = makeLLVMLabelForTok("IfStmntNoElse:end", getEndToken());
    mEndBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, endBBLbl, parentFn);
    WC_ASSERT(mEndBasicBlock);

    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenNode.codegen(cgCtx)) {
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
                             IBasicCodegenNode & thenNode,
                             IfStmnt & outerIfStmnt,
                             const Token & startToken)
:
    IfStmnt(ifExpr, thenNode, startToken),
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
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    WC_ASSERT(ifBB);
    
    // Create the 'then' and 'outer if' blocks:
    ASTNode & thenASTNode = dynamic_cast<ASTNode&>(mThenNode);
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntElseIf:then", thenASTNode.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    std::string outerIfBBLbl = makeLLVMLabelForTok("IfStmntElseIf:outer_if", mElseIfStmnt.getStartToken());
    llvm::BasicBlock * outerIfBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, outerIfBBLbl, parentFn);
    WC_ASSERT(outerIfBB);
    
    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenNode.codegen(cgCtx)) {
        return false;
    }
    
    // Codegen the 'outer if' block
    cgCtx.irBuilder.SetInsertPoint(outerIfBB);
    
    if (!mElseIfStmnt.codegen(cgCtx)) {
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
                         IBasicCodegenNode & thenNode,
                         IBasicCodegenNode & elseNode,
                         const Token & startToken,
                         const Token & endToken)
:
    IfStmnt(ifExpr, thenNode, startToken),
    mElseNode(elseNode),
    mEndToken(endToken)
{
    // Need to do a tricky cast for this:
    ASTNode * elseASTNode = dynamic_cast<ASTNode*>(&elseNode);
    WC_ASSERT(elseASTNode);
    elseASTNode->mParent = this;
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
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    
    // Create the 'then', 'else' and 'end' blocks:
    ASTNode & thenASTNode = dynamic_cast<ASTNode&>(mThenNode);
    std::string thenBBLbl = makeLLVMLabelForTok("IfStmntElse:then", thenASTNode.getStartToken());
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, thenBBLbl, parentFn);
    WC_ASSERT(thenBB);
    
    ASTNode & elseASTNode = dynamic_cast<ASTNode&>(mElseNode);
    std::string elseBBLbl = makeLLVMLabelForTok("IfStmntElse:else", elseASTNode.getStartToken());
    llvm::BasicBlock * elseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, elseBBLbl, parentFn);
    WC_ASSERT(elseBB);
    
    std::string endBBLbl = makeLLVMLabelForTok("IfStmntElse:end", getEndToken());
    mEndBasicBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, endBBLbl, parentFn);
    WC_ASSERT(mEndBasicBlock);
    
    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mThenNode.codegen(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(mEndBasicBlock);
    
    // Codegen the 'else' block
    cgCtx.irBuilder.SetInsertPoint(elseBB);
    
    if (!mElseNode.codegen(cgCtx)) {
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
