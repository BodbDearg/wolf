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
    
    // TOOD: parse other if variants
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate 'if' block!");
        return nullptr;
    }
    
    const Token * endToken = tokenPtr;
    ++tokenPtr;                             // Skip 'end'

    return new IfStmntNoElse(*ifExpr, *innerScope, *startToken, *endToken);
}

//-----------------------------------------------------------------------------
// IfStmntNoElse
//-----------------------------------------------------------------------------

IfStmntNoElse::IfStmntNoElse(AssignExpr & ifExpr,
                             Scope & innerScope,
                             const Token & startToken,
                             const Token & endToken)
:
    mIfExpr(ifExpr),
    mInnerScope(innerScope),
    mStartToken(startToken),
    mEndToken(endToken)
{
    mIfExpr.mParent = this;
    mInnerScope.mParent = this;
}

const Token & IfStmntNoElse::getStartToken() const {
    return mStartToken;
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
    
    // Create the 'then' and 'end' blocks:
    llvm::BasicBlock * thenBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "IfStmntNoElse:then", parentFn);
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "if_end", parentFn);

    // Codegen the 'then' block
    cgCtx.irBuilder.SetInsertPoint(thenBB);
    
    if (!mInnerScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(endBB);
    
    // Generate the branch
    cgCtx.irBuilder.SetInsertPoint(ifBB);
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(ifExprResult, thenBB, endBB);
    WC_GUARD_ASSERT(branch, false);
    
    // Switch back to the end block:
    cgCtx.irBuilder.SetInsertPoint(endBB);
    
    // All good!
    return true;
}

WC_END_NAMESPACE
