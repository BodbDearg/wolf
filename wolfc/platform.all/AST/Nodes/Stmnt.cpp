#include "Stmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssertStmnt.hpp"
#include "AssignExpr.hpp"
#include "BreakStmnt.hpp"
#include "IfStmnt.hpp"
#include "LinearAlloc.hpp"
#include "LoopStmnt.hpp"
#include "NextStmnt.hpp"
#include "NoOpStmnt.hpp"
#include "PrintStmnt.hpp"
#include "ReturnStmnt.hpp"
#include "ScopeStmnt.hpp"
#include "VarDecl.hpp"
#include "WhileStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

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
    #warning Handle newlines during parsing
    // Parse 'noop' statment if ahead
    if (NoOpStmnt::peek(parseCtx.tok())) {
        NoOpStmnt * noOpStmnt = NoOpStmnt::parse(parseCtx);
        WC_GUARD(noOpStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntNoOpStmnt, *noOpStmnt);
    }
    
    // Parse print expression if ahead
    if (PrintStmnt::peek(parseCtx.tok())) {
        PrintStmnt * printStmnt = PrintStmnt::parse(parseCtx);
        WC_GUARD(printStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntPrintStmnt, *printStmnt);
    }
    
    // Parse assert expression if ahead
    if (AssertStmnt::peek(parseCtx.tok())) {
        AssertStmnt * assertStmnt = AssertStmnt::parse(parseCtx);
        WC_GUARD(assertStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntAssertStmnt, *assertStmnt);
    }
    
    // Parse var decl if ahead
    if (VarDecl::peek(parseCtx.tok())) {
        VarDecl * varDecl = VarDecl::parse(parseCtx);
        WC_GUARD(varDecl, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntVarDecl, *varDecl);
    }
    
    // Parse if statement if ahead
    if (IfStmnt::peek(parseCtx.tok())) {
        IfStmnt * ifStmnt = IfStmnt::parse(parseCtx);
        WC_GUARD(ifStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntIfStmnt, *ifStmnt);
    }
    
    // Parse while statement if ahead
    if (WhileStmnt::peek(parseCtx.tok())) {
        WhileStmnt * whileStmnt = WhileStmnt::parse(parseCtx);
        WC_GUARD(whileStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntWhileStmnt, *whileStmnt);
    }
    
    // Parse loop statement if ahead
    if (LoopStmnt::peek(parseCtx.tok())) {
        LoopStmnt * loopStmnt = LoopStmnt::parse(parseCtx);
        WC_GUARD(loopStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntLoopStmnt, *loopStmnt);
    }
    
    // Parse break statement if ahead
    if (BreakStmnt::peek(parseCtx.tok())) {
        BreakStmnt * breakStmnt = BreakStmnt::parse(parseCtx);
        WC_GUARD(breakStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntBreakStmnt, *breakStmnt);
    }
    
    // Parse scope statement if ahead
    if (ScopeStmnt::peek(parseCtx.tok())) {
        ScopeStmnt * scopeStmnt = ScopeStmnt::parse(parseCtx);
        WC_GUARD(scopeStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntScopeStmnt, *scopeStmnt);
    }
    
    // Parse next statement if ahead
    if (NextStmnt::peek(parseCtx.tok())) {
        NextStmnt * nextStmnt = NextStmnt::parse(parseCtx);
        WC_GUARD(nextStmnt, nullptr);
        return WC_NEW_AST_NODE(parseCtx, StmntNextStmnt, *nextStmnt);
    }
    
    // Parse return statement if ahead
    if (ReturnStmnt::peek(parseCtx.tok())) {
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

void StmntNoOpStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntNoOpStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntNoOpStmnt::getEndToken() const {
    return mStmnt.getEndToken();
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

void StmntPrintStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntPrintStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntPrintStmnt::getEndToken() const {
    return mStmnt.getEndToken();
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

void StmntAssertStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntAssertStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntAssertStmnt::getEndToken() const {
    return mStmnt.getEndToken();
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

void StmntIfStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntIfStmnt::getStartToken() const {
    return mIfStmnt.getStartToken();
}

const Token & StmntIfStmnt::getEndToken() const {
    return mIfStmnt.getEndToken();
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

void StmntWhileStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntWhileStmnt::getStartToken() const {
    return mWhileStmnt.getStartToken();
}

const Token & StmntWhileStmnt::getEndToken() const {
    return mWhileStmnt.getEndToken();
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

void StmntLoopStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntLoopStmnt::getStartToken() const {
    return mLoopStmnt.getStartToken();
}

const Token & StmntLoopStmnt::getEndToken() const {
    return mLoopStmnt.getEndToken();
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

void StmntScopeStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntScopeStmnt::getStartToken() const {
    return mScopeStmnt.getStartToken();
}

const Token & StmntScopeStmnt::getEndToken() const {
    return mScopeStmnt.getEndToken();
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

void StmntBreakStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntBreakStmnt::getStartToken() const {
    return mBreakStmnt.getStartToken();
}

const Token & StmntBreakStmnt::getEndToken() const {
    return mBreakStmnt.getEndToken();
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

void StmntNextStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntNextStmnt::getStartToken() const {
    return mNextStmnt.getStartToken();
}

const Token & StmntNextStmnt::getEndToken() const {
    return mNextStmnt.getEndToken();
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

void StmntReturnStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntReturnStmnt::getStartToken() const {
    return mReturnStmnt.getStartToken();
}

const Token & StmntReturnStmnt::getEndToken() const {
    return mReturnStmnt.getEndToken();
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

void StmntVarDecl::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntVarDecl::getStartToken() const {
    return mDecl.getStartToken();
}

const Token & StmntVarDecl::getEndToken() const {
    return mDecl.getEndToken();
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

void StmntAssignExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StmntAssignExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & StmntAssignExpr::getEndToken() const {
    return mExpr.getEndToken();
}

bool StmntAssignExpr::allCodepathsHaveUncondRet() const {
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
