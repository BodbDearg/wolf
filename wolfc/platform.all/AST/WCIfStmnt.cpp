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
    return tokenPtr->type == TokenType::kIf;
}

IfStmnt * IfStmnt::parse(const Token *& tokenPtr) {
    // Parse the initial 'if'
    if (tokenPtr->type != TokenType::kIf) {
        parseError(*tokenPtr, "If statement expected!");
        return nullptr;
    }
    
    // Skip 'if' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(ifExpr, nullptr);
    
    // Expect 'then'
    if (tokenPtr->type != TokenType::kThen) {
        parseError(*tokenPtr, "Expect 'then' following if statement condition!");
        return nullptr;
    }
    
    // Skip 'then'
    ++tokenPtr;
    
    // Expect scope following:
    Scope * thenScope = Scope::parse(tokenPtr);
    WC_GUARD(thenScope, nullptr);
    
    // See if an 'else' or 'else if' statement follows:
    if (tokenPtr->type == TokenType::kElse) {
        // Else or else if follows: skip 'else' token:
        ++tokenPtr;
        
        // See if there is a further if following:
        if (IfStmnt::peek(tokenPtr)) {
            // 'if then else if' statement: parse the if statement following the 'else':
            IfStmnt * outerIfStmnt = IfStmnt::parse(tokenPtr);
            WC_GUARD(outerIfStmnt, nullptr);
            
            // Done, return the parsed statement:
            return new IfStmntElseIf(*ifExpr, *thenScope, *outerIfStmnt, *startToken);
        }
        else {
            // 'if then else' statement: parse the scope for the 'else' block:
            Scope * elseScope = Scope::parse(tokenPtr);
            WC_GUARD(elseScope, nullptr);
            
            // Else block should end on an 'end' token:
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
    }
    else {
        // 'If then else' type statement: expect closing 'end'
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
    // The if expression must first evaluate to a bool!
    const DataType & ifExprDataType = mIfExpr.dataType();
    
    if (!ifExprDataType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool))) {
        compileError("Expression for if statement must evaluate to type 'bool', not '%s'!",
                     ifExprDataType.name());
        
        return false;
    }
    
    // Alright, generate the code for that:
    llvm::Value * ifExprResult = mIfExpr.codegenExprEval(cgCtx);
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
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, mEndBasicBlock);
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    
    // All good!
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
    // The if expression must first evaluate to a bool!
    const DataType & ifExprDataType = mIfExpr.dataType();
    
    if (!ifExprDataType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool))) {
        compileError("Expression for if statement must evaluate to type 'bool', not '%s'!",
                     ifExprDataType.name());
        
        return false;
    }
    
    // Alright, generate the code for that:
    llvm::Value * ifExprResult = mIfExpr.codegenExprEval(cgCtx);
    WC_GUARD(ifExprResult, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Save the current insert block:
    llvm::BasicBlock * ifBB = cgCtx.irBuilder.GetInsertBlock();
    
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
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, outerIfBB);
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    
    // All good!
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
    // The if expression must first evaluate to a bool!
    const DataType & ifExprDataType = mIfExpr.dataType();
    
    if (!ifExprDataType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool))) {
        compileError("Expression for if statement must evaluate to type 'bool', not '%s'!",
                     ifExprDataType.name());
        
        return false;
    }
    
    // Alright, generate the code for that:
    llvm::Value * ifExprResult = mIfExpr.codegenExprEval(cgCtx);
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
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, elseBB);
    WC_ASSERT(branch);
    
    // Switch back to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(mEndBasicBlock);
    
    // All good!
    return true;
}

WC_END_NAMESPACE
