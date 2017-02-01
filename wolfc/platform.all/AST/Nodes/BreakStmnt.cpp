#include "BreakStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Assert.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "IRepeatableStmnt.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BreakStmnt
//-----------------------------------------------------------------------------
bool BreakStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kBreak;
}

BreakStmnt * BreakStmnt::parse(ParseCtx & parseCtx) {
    // Check the basics
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected break statement!");
        return nullptr;
    }
    
    // Consume 'break' and save token for later:
    const Token * breakTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // See whether 'if' or 'unless' follow, in which case the 'break' statement is conditional:
    if (parseCtx.tok()->type == TokenType::kIf || parseCtx.tok()->type == TokenType::kUnless) {
        // Parse the condition token:
        const Token * condTok = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the condition assign expression:
        AssignExpr * condExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(condExpr, nullptr);
        
        // 'break' with a condition:
        return WC_NEW_AST_NODE(parseCtx, BreakStmntWithCond, *breakTok, *condTok, *condExpr);
    }
    
    // 'break' without a condition:
    return WC_NEW_AST_NODE(parseCtx, BreakStmntNoCond, *breakTok);
}

BreakStmnt::BreakStmnt(const Token & breakToken) : mBreakToken(breakToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & BreakStmnt::getStartToken() const {
    return mBreakToken;
}

bool BreakStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

#warning FIXME - Codegen
#if 0
bool BreakStmnt::deferredCodegen(CodegenCtx & cgCtx) {
    // Get the parent repeatable statement:
    IRepeatableStmnt * parentRepeatableStmnt = firstParentOfType<IRepeatableStmnt>();
    
    if (!parentRepeatableStmnt) {
        compileError("'break' statement must have a parent repeatable block/loop! 'break' cannot be used outside of loops!");
        return false;
    }
    
    // Generate the jump to past the end of the parent loop:
    cgCtx.irBuilder.SetInsertPoint(mBreakBlock);
    cgCtx.irBuilder.CreateBr(parentRepeatableStmnt->getBreakStmntTargetBlock());
    return true;
}
#endif

//-----------------------------------------------------------------------------
// BreakStmntNoCond
//-----------------------------------------------------------------------------
BreakStmntNoCond::BreakStmntNoCond(const Token & breakToken) : BreakStmnt(breakToken) {
    WC_EMPTY_FUNC_BODY();
}

void BreakStmntNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & BreakStmntNoCond::getEndToken() const {
    return mBreakToken;
}

//-----------------------------------------------------------------------------
// BreakStmntWithCond
//-----------------------------------------------------------------------------
BreakStmntWithCond::BreakStmntWithCond(const Token & breakToken,
                                       const Token & condToken,
                                       AssignExpr & condExpr)
:
    BreakStmnt(breakToken),
    mCondToken(condToken),
    mCondExpr(condExpr)
{
    mCondExpr.mParent = this;
}

void BreakStmntWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & BreakStmntWithCond::getEndToken() const {
    return mCondExpr.getEndToken();
}

#warning FIXME - Codegen
#if 0
bool BreakStmntWithCond::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create the basic block for the break code
    mBreakBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "BreakStmntWithCond:break", parentFn);
    WC_ASSERT(mBreakBlock);
    
    // Create the basic block for the continue code:
    mContinueBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "BreakStmntWithCond:continue", parentFn);
    WC_ASSERT(mContinueBlock);
    
    // The assign expression must evaluate to bool:
    if (!mCondExpr.dataType().isBool()) {
        compileError("Condition for 'break' statement must evaluate to type 'bool', not '%s'!",
                     mCondExpr.dataType().name().c_str());
        
        return false;
    }
    
    // Generate the value for the condition assign expression:
    llvm::Value * condResult = mCondExpr.codegenExprEval(cgCtx);
    WC_GUARD(condResult, false);
    
    // Point the previous block to this new basic block:
    if (isIfCondInverted()) {
        cgCtx.irBuilder.CreateCondBr(condResult, mContinueBlock, mBreakBlock);
    }
    else {
        cgCtx.irBuilder.CreateCondBr(condResult, mBreakBlock, mContinueBlock);
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

bool BreakStmntWithCond::isIfCondInverted() const {
    return mCondToken.type == TokenType::kUnless;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
