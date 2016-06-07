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
    
    const Token * startToken = tokenPtr;
    ++tokenPtr;                             // Skip 'if'
    
    // Parse the if condition:
    AssignExpr * ifExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(ifExpr, nullptr);
    
    // Expect 'then'
    if (tokenPtr->type != TokenType::kThen) {
        parseError(*tokenPtr, "Expect 'then' following if statement condition!");
        return nullptr;
    }
    
    ++tokenPtr;     // Skip 'then'
    
    // Expect scope following:
    Scope * innerScope = Scope::parse(tokenPtr);
    WC_GUARD(innerScope, nullptr);
    
    // See if an 'else' or 'else if' statement follows:
    if (tokenPtr->type == TokenType::kElse) {
        ++tokenPtr;     // Skip 'else'
        
        if (IfStmnt::peek(tokenPtr)) {
            // 'If then else if' type statement: parse the following if:
            IfStmnt * outerIfStmnt = IfStmnt::parse(tokenPtr);
            WC_GUARD(outerIfStmnt, nullptr);
            
            // Join it all up:
            return new IfStmntElseIf(*ifExpr, *innerScope, *outerIfStmnt, *startToken);
        }
        else {
            // 'if then else' type stement: parse the scope for the 'else' blocK:
            WC_RAISE_ASSERTION("NOT IMPLEMENTED");
            return nullptr;
        }
    }
    else {
        // 'If then else' type statement: expect closing 'end'
        if (tokenPtr->type != TokenType::kEnd) {
            parseError(*tokenPtr, "'end' expected to terminate 'if' block!");
            return nullptr;
        }
        
        const Token * endToken = tokenPtr;
        ++tokenPtr;                             // Skip 'end'
        
        return new IfStmntNoElse(*ifExpr, *innerScope, *startToken, *endToken);
    }
}

IfStmnt::IfStmnt(AssignExpr & ifExpr,
                 Scope & innerScope,
                 const Token & startToken)
:
    mIfExpr(ifExpr),
    mInnerScope(innerScope),
    mStartToken(startToken)
{
    mIfExpr.mParent = this;
    mInnerScope.mParent = this;
}

const Token & IfStmnt::getStartToken() const {
    return mStartToken;
}

//-----------------------------------------------------------------------------
// IfStmntNoElse
//-----------------------------------------------------------------------------

IfStmntNoElse::IfStmntNoElse(AssignExpr & ifExpr,
                             Scope & innerScope,
                             const Token & startToken,
                             const Token & endToken)
:
    IfStmnt(ifExpr, innerScope, startToken),
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
    
    if (!mInnerScope.codegenStmnt(cgCtx)) {
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
                             Scope & innerScope,
                             IfStmnt & outerIfStmnt,
                             const Token & startToken)
:
    IfStmnt(ifExpr, innerScope, startToken),
    mOuterIfStmnt(outerIfStmnt)
{
    mOuterIfStmnt.mParent = this;
}

const Token & IfStmntElseIf::getEndToken() const {
    return mOuterIfStmnt.getEndToken();
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
    
    // Create the 'then', 'outer if' and 'end' blocks:
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElseIf:then", parentFn);
    WC_ASSERT(thenBB);
    
    llvm::BasicBlock * outerIfBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntElseIf:outerIf", parentFn);
    WC_ASSERT(outerIfBB);
    
    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mInnerScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Codegen the 'outer if' block
    cgCtx.irBuilder.SetInsertPoint(outerIfBB);
    
    if (!mOuterIfStmnt.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Expect the outer if to have an 'end' basic block. Tie the end of the 'then' block to that:
    WC_ASSERT(mOuterIfStmnt.mEndBasicBlock);
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    mEndBasicBlock = mOuterIfStmnt.mEndBasicBlock;
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

WC_END_NAMESPACE
