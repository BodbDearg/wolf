#include "WCStmnt.hpp"

#include "WCAssignStmnt.hpp"
#include "WCBreakStmnt.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIfStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCLoopStmnt.hpp"
#include "WCNextStmnt.hpp"
#include "WCNopStmnt.hpp"
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
    return  NopStmnt::peek(tokenPtr) ||
            PrintStmnt::peek(tokenPtr) ||
            VarDecl::peek(tokenPtr) ||
            IfStmnt::peek(tokenPtr) ||
            WhileStmnt::peek(tokenPtr) ||
            LoopStmnt::peek(tokenPtr) ||
            ScopeStmnt::peek(tokenPtr) ||
            BreakStmnt::peek(tokenPtr) ||
            NextStmnt::peek(tokenPtr) ||
            ReturnStmnt::peek(tokenPtr) ||
            AssignStmnt::peek(tokenPtr);
}
    
Stmnt * Stmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse 'nop' statment if ahead
    if (NopStmnt::peek(tokenPtr)) {
        NopStmnt * nopStmnt = NopStmnt::parse(tokenPtr, alloc);
        WC_GUARD(nopStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntNopStmnt, *nopStmnt);
    }
    
    // Parse print expression if ahead
    if (PrintStmnt::peek(tokenPtr)) {
        PrintStmnt * printStmnt = PrintStmnt::parse(tokenPtr, alloc);
        WC_GUARD(printStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntPrintStmnt, *printStmnt);
    }
    
    // Parse var decl if ahead
    if (VarDecl::peek(tokenPtr)) {
        VarDecl * varDecl = VarDecl::parse(tokenPtr, alloc);
        WC_GUARD(varDecl, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntVarDecl, *varDecl);
    }
    
    // Parse if statement if ahead
    if (IfStmnt::peek(tokenPtr)) {
        IfStmnt * ifStmnt = IfStmnt::parse(tokenPtr, alloc);
        WC_GUARD(ifStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntIfStmnt, *ifStmnt);
    }
    
    // Parse while statement if ahead
    if (WhileStmnt::peek(tokenPtr)) {
        WhileStmnt * whileStmnt = WhileStmnt::parse(tokenPtr, alloc);
        WC_GUARD(whileStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntWhileStmnt, *whileStmnt);
    }
    
    // Parse loop statement if ahead
    if (LoopStmnt::peek(tokenPtr)) {
        LoopStmnt * loopStmnt = LoopStmnt::parse(tokenPtr, alloc);
        WC_GUARD(loopStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntLoopStmnt, *loopStmnt);
    }
    
    // Parse break statement if ahead
    if (BreakStmnt::peek(tokenPtr)) {
        BreakStmnt * breakStmnt = BreakStmnt::parse(tokenPtr, alloc);
        WC_GUARD(breakStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntBreakStmnt, *breakStmnt);
    }
    
    // Parse scope statement if ahead
    if (ScopeStmnt::peek(tokenPtr)) {
        ScopeStmnt * scopeStmnt = ScopeStmnt::parse(tokenPtr, alloc);
        WC_GUARD(scopeStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntScopeStmnt, *scopeStmnt);
    }
    
    // Parse next statement if ahead
    if (NextStmnt::peek(tokenPtr)) {
        NextStmnt * nextStmnt = NextStmnt::parse(tokenPtr, alloc);
        WC_GUARD(nextStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntNextStmnt, *nextStmnt);
    }
    
    // Parse return statement if ahead
    if (ReturnStmnt::peek(tokenPtr)) {
        ReturnStmnt * returnStmnt = ReturnStmnt::parse(tokenPtr, alloc);
        WC_GUARD(returnStmnt, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntReturnStmnt, *returnStmnt);
    }
    
    // Otherwise parse assign statement
    AssignStmnt * assignStmnt = AssignStmnt::parse(tokenPtr, alloc);
    WC_GUARD(assignStmnt, nullptr);
    return WC_NEW_AST_NODE(alloc, StmntAssignStmnt, *assignStmnt);
}

//-----------------------------------------------------------------------------
// StmntNopStmnt
//-----------------------------------------------------------------------------

StmntNopStmnt::StmntNopStmnt(NopStmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & StmntNopStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntNopStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}

bool StmntNopStmnt::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
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

//-----------------------------------------------------------------------------
// StmntAssignStmnt
//-----------------------------------------------------------------------------

StmntAssignStmnt::StmntAssignStmnt(AssignStmnt & expr) : mStmnt(expr) {
    mStmnt.mParent = this;
}

const Token & StmntAssignStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntAssignStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}
    
bool StmntAssignStmnt::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
}

WC_END_NAMESPACE
