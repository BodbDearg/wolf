#include "WCScopeStmnt.hpp"

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

bool ScopeStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kScope;
}

ScopeStmnt * ScopeStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'scope' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("'scope' statement expected!");
        return nullptr;
    }
    
    // Skip the 'scope' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the body scope:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    
    // Must be terminated by an 'end' token
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate 'scope' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(parseCtx, ScopeStmnt, *startToken, *bodyScope, *endToken);
}

ScopeStmnt::ScopeStmnt(const Token & startToken, Scope & bodyScope, const Token & endToken) :
    mStartToken(startToken),
    mBodyScope(bodyScope),
    mEndToken(endToken)
{
    mBodyScope.mParent = this;
}

void ScopeStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ScopeStmnt::getStartToken() const {
    return mStartToken;
}

const Token & ScopeStmnt::getEndToken() const {
    return mEndToken;
}

#warning FIXME - Codegen
#if 0
bool ScopeStmnt::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the 'scope' main block.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    llvm::BasicBlock * startBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ScopeStmnt:block", parentFn);
    WC_ASSERT(startBB);
    cgCtx.irBuilder.CreateBr(startBB);

    // Codegen the main body scope:
    cgCtx.irBuilder.SetInsertPoint(startBB);
    
    if (!mBodyScope.codegen(cgCtx)) {
        return false;
    }
    
    // Create the end basic block and make the current block branch to it to terminate it:
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ScopeStmnt:end", parentFn);
    WC_ASSERT(endBB);
    cgCtx.irBuilder.CreateBr(endBB);
    
    // Switch to inserting code at the end block:
    cgCtx.irBuilder.SetInsertPoint(endBB);
    return true;
}
#endif

bool ScopeStmnt::allCodepathsHaveUncondRet() const {
    return mBodyScope.allCodepathsHaveUncondRet();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
