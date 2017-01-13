#include "WCStmnt.hpp"

#include "WCAssertStmnt.hpp"
#include "WCAssignExpr.hpp"
#include "WCBreakStmnt.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIfStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCLoopStmnt.hpp"
#include "WCNextStmnt.hpp"
#include "WCNoOpStmnt.hpp"
#include "WCParseCtx.hpp"
#include "WCPrintStmnt.hpp"
#include "WCReturnStmnt.hpp"
#include "WCScopeStmnt.hpp"
#include "WCVarDecl.hpp"
#include "WCWhileStmnt.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Stmnt
//-----------------------------------------------------------------------------
bool Stmnt::peek(const Token * tokenPtr) {
    return  NoOpStmnt::peek(tokenPtr) ||
            PrintStmnt::peek(tokenPtr) ||
            AssertStmnt::peek(tokenPtr) ||
            VarDecl::peek(tokenPtr) ||
            IfStmnt::peek(tokenPtr) ||
            WhileStmnt::peek(tokenPtr) ||
            LoopStmnt::peek(tokenPtr) ||
            ScopeStmnt::peek(tokenPtr) ||
            BreakStmnt::peek(tokenPtr) ||
            NextStmnt::peek(tokenPtr) ||
            ReturnStmnt::peek(tokenPtr) ||
            AssignExpr::peek(tokenPtr);
}
    
Stmnt * Stmnt::parse(ParseCtx & parseCtx) {
    // Parse 'noop' statment if ahead
    if (NoOpStmnt::peek(parseCtx.curTok)) {
        NoOpStmnt * noOpStmnt = NoOpStmnt::parse(parseCtx);
        WC_GUARD(noOpStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntNoOpStmnt, *noOpStmnt);
    }
    
    // Parse print expression if ahead
    if (PrintStmnt::peek(parseCtx.curTok)) {
        PrintStmnt * printStmnt = PrintStmnt::parse(parseCtx);
        WC_GUARD(printStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntPrintStmnt, *printStmnt);
    }
    
    // Parse assert expression if ahead
    if (AssertStmnt::peek(parseCtx.curTok)) {
        AssertStmnt * assertStmnt = AssertStmnt::parse(parseCtx);
        WC_GUARD(assertStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntAssertStmnt, *assertStmnt);
    }
    
    // Parse var decl if ahead
    if (VarDecl::peek(parseCtx.curTok)) {
        VarDecl * varDecl = VarDecl::parse(parseCtx);
        WC_GUARD(varDecl, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntVarDecl, *varDecl);
    }
    
    // Parse if statement if ahead
    if (IfStmnt::peek(parseCtx.curTok)) {
        IfStmnt * ifStmnt = IfStmnt::parse(parseCtx);
        WC_GUARD(ifStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntIfStmnt, *ifStmnt);
    }
    
    // Parse while statement if ahead
    if (WhileStmnt::peek(parseCtx.curTok)) {
        WhileStmnt * whileStmnt = WhileStmnt::parse(parseCtx);
        WC_GUARD(whileStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntWhileStmnt, *whileStmnt);
    }
    
    // Parse loop statement if ahead
    if (LoopStmnt::peek(parseCtx.curTok)) {
        LoopStmnt * loopStmnt = LoopStmnt::parse(parseCtx);
        WC_GUARD(loopStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntLoopStmnt, *loopStmnt);
    }
    
    // Parse break statement if ahead
    if (BreakStmnt::peek(parseCtx.curTok)) {
        BreakStmnt * breakStmnt = BreakStmnt::parse(parseCtx);
        WC_GUARD(breakStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntBreakStmnt, *breakStmnt);
    }
    
    // Parse scope statement if ahead
    if (ScopeStmnt::peek(parseCtx.curTok)) {
        ScopeStmnt * scopeStmnt = ScopeStmnt::parse(parseCtx);
        WC_GUARD(scopeStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntScopeStmnt, *scopeStmnt);
    }
    
    // Parse next statement if ahead
    if (NextStmnt::peek(parseCtx.curTok)) {
        NextStmnt * nextStmnt = NextStmnt::parse(parseCtx);
        WC_GUARD(nextStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntNextStmnt, *nextStmnt);
    }
    
    // Parse return statement if ahead
    if (ReturnStmnt::peek(parseCtx.curTok)) {
        ReturnStmnt * returnStmnt = ReturnStmnt::parse(parseCtx);
        WC_GUARD(returnStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntReturnStmnt, *returnStmnt);
    }
    
    // Otherwise parse assign expression
    AssignExpr * assignExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(assignExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, StmntAssignExpr, *assignExpr);
}

//-----------------------------------------------------------------------------
// StmntNoOpStmnt
//-----------------------------------------------------------------------------
StmntNoOpStmnt::StmntNoOpStmnt(NoOpStmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & StmntNoOpStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntNoOpStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}

bool StmntNoOpStmnt::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
}

bool StmntNoOpStmnt::allCodepathsHaveUncondRet() const {
    return mStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntPrintStmnt
//-----------------------------------------------------------------------------
StmntPrintStmnt::StmntPrintStmnt(PrintStmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & StmntPrintStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntPrintStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}

bool StmntPrintStmnt::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
}

bool StmntPrintStmnt::allCodepathsHaveUncondRet() const {
    return mStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntAssertStmnt
//-----------------------------------------------------------------------------
StmntAssertStmnt::StmntAssertStmnt(AssertStmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & StmntAssertStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntAssertStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}

bool StmntAssertStmnt::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
}

bool StmntAssertStmnt::allCodepathsHaveUncondRet() const {
    return mStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntIfStmnt
//-----------------------------------------------------------------------------
StmntIfStmnt::StmntIfStmnt(IfStmnt & ifStmnt) : mIfStmnt(ifStmnt) {
    mIfStmnt.mParent = this;
}

const Token & StmntIfStmnt::getStartToken() const {
    return mIfStmnt.getStartToken();
}

const Token & StmntIfStmnt::getEndToken() const {
    return mIfStmnt.getEndToken();
}

bool StmntIfStmnt::codegen(CodegenCtx & cgCtx) {
    return mIfStmnt.codegen(cgCtx);
}

bool StmntIfStmnt::allCodepathsHaveUncondRet() const {
    return mIfStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntWhileStmnt
//-----------------------------------------------------------------------------
StmntWhileStmnt::StmntWhileStmnt(WhileStmnt & whileStmnt) : mWhileStmnt(whileStmnt) {
    mWhileStmnt.mParent = this;
}

const Token & StmntWhileStmnt::getStartToken() const {
    return mWhileStmnt.getStartToken();
}

const Token & StmntWhileStmnt::getEndToken() const {
    return mWhileStmnt.getEndToken();
}

bool StmntWhileStmnt::codegen(CodegenCtx & cgCtx) {
    return mWhileStmnt.codegen(cgCtx);
}

bool StmntWhileStmnt::allCodepathsHaveUncondRet() const {
    return mWhileStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntLoopStmnt
//-----------------------------------------------------------------------------
StmntLoopStmnt::StmntLoopStmnt(LoopStmnt & loopStmnt) : mLoopStmnt(loopStmnt) {
    mLoopStmnt.mParent = this;
}

const Token & StmntLoopStmnt::getStartToken() const {
    return mLoopStmnt.getStartToken();
}

const Token & StmntLoopStmnt::getEndToken() const {
    return mLoopStmnt.getEndToken();
}

bool StmntLoopStmnt::codegen(CodegenCtx & cgCtx) {
    return mLoopStmnt.codegen(cgCtx);
}

bool StmntLoopStmnt::allCodepathsHaveUncondRet() const {
    return mLoopStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntScopeStmnt
//-----------------------------------------------------------------------------
StmntScopeStmnt::StmntScopeStmnt(ScopeStmnt & scopeStmnt) : mScopeStmnt(scopeStmnt) {
    mScopeStmnt.mParent = this;
}

const Token & StmntScopeStmnt::getStartToken() const {
    return mScopeStmnt.getStartToken();
}

const Token & StmntScopeStmnt::getEndToken() const {
    return mScopeStmnt.getEndToken();
}

bool StmntScopeStmnt::codegen(CodegenCtx & cgCtx) {
    return mScopeStmnt.codegen(cgCtx);
}

bool StmntScopeStmnt::allCodepathsHaveUncondRet() const {
    return mScopeStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntBreakStmnt
//-----------------------------------------------------------------------------
StmntBreakStmnt::StmntBreakStmnt(BreakStmnt & breakStmnt) : mBreakStmnt(breakStmnt) {
    mBreakStmnt.mParent = this;
}

const Token & StmntBreakStmnt::getStartToken() const {
    return mBreakStmnt.getStartToken();
}

const Token & StmntBreakStmnt::getEndToken() const {
    return mBreakStmnt.getEndToken();
}

bool StmntBreakStmnt::codegen(CodegenCtx & cgCtx) {
    return mBreakStmnt.codegen(cgCtx);
}

bool StmntBreakStmnt::allCodepathsHaveUncondRet() const {
    return mBreakStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntNextStmnt
//-----------------------------------------------------------------------------
StmntNextStmnt::StmntNextStmnt(NextStmnt & nextStmnt) : mNextStmnt(nextStmnt) {
    mNextStmnt.mParent = this;
}

const Token & StmntNextStmnt::getStartToken() const {
    return mNextStmnt.getStartToken();
}

const Token & StmntNextStmnt::getEndToken() const {
    return mNextStmnt.getEndToken();
}

bool StmntNextStmnt::codegen(CodegenCtx & cgCtx) {
    return mNextStmnt.codegen(cgCtx);
}

bool StmntNextStmnt::allCodepathsHaveUncondRet() const {
    return mNextStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntReturnStmnt
//-----------------------------------------------------------------------------
StmntReturnStmnt::StmntReturnStmnt(ReturnStmnt & returnStmnt) : mReturnStmnt(returnStmnt) {
    mReturnStmnt.mParent = this;
}

const Token & StmntReturnStmnt::getStartToken() const {
    return mReturnStmnt.getStartToken();
}

const Token & StmntReturnStmnt::getEndToken() const {
    return mReturnStmnt.getEndToken();
}

bool StmntReturnStmnt::codegen(CodegenCtx & cgCtx) {
    return mReturnStmnt.codegen(cgCtx);
}

bool StmntReturnStmnt::allCodepathsHaveUncondRet() const {
    return mReturnStmnt.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntVarDecl
//-----------------------------------------------------------------------------
StmntVarDecl::StmntVarDecl(VarDecl & decl) : mDecl(decl) {
    mDecl.mParent = this;
}

const Token & StmntVarDecl::getStartToken() const {
    return mDecl.getStartToken();
}

const Token & StmntVarDecl::getEndToken() const {
    return mDecl.getEndToken();
}

bool StmntVarDecl::codegen(CodegenCtx & cgCtx) {
    return mDecl.codegen(cgCtx);
}

bool StmntVarDecl::allCodepathsHaveUncondRet() const {
    return mDecl.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// StmntAssignExpr
//-----------------------------------------------------------------------------
StmntAssignExpr::StmntAssignExpr(AssignExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & StmntAssignExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & StmntAssignExpr::getEndToken() const {
    return mExpr.getEndToken();
}
    
bool StmntAssignExpr::codegen(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx) != nullptr;
}

bool StmntAssignExpr::allCodepathsHaveUncondRet() const {
    return false;
}

WC_END_NAMESPACE
