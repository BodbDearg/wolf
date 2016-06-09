#include "WCStmnt.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIfStmnt.hpp"
#include "WCNopStmnt.hpp"
#include "WCPrintStmnt.hpp"
#include "WCVarDecl.hpp"
#include "WCWhileStmnt.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------

bool Stmnt::peek(const Token * tokenPtr) {
    return  NopStmnt::peek(tokenPtr) ||
            PrintStmnt::peek(tokenPtr) ||
            VarDecl::peek(tokenPtr) ||
            IfStmnt::peek(tokenPtr) ||
            WhileStmnt::peek(tokenPtr) ||
            AssignExpr::peek(tokenPtr);
}
    
Stmnt * Stmnt::parse(const Token *& tokenPtr) {
    // Parse 'nop' statment if ahead
    if (NopStmnt::peek(tokenPtr)) {
        NopStmnt * nopStmnt = NopStmnt::parse(tokenPtr);
        WC_GUARD(nopStmnt, nullptr);
        return new StmntNopStmnt(*nopStmnt);
    }
    
    // Parse print expression if ahead
    if (PrintStmnt::peek(tokenPtr)) {
        PrintStmnt * printStmnt = PrintStmnt::parse(tokenPtr);
        WC_GUARD(printStmnt, nullptr);
        return new StmntPrintStmnt(*printStmnt);
    }
    
    // Parse var decl if ahead
    if (VarDecl::peek(tokenPtr)) {
        VarDecl * varDecl = VarDecl::parse(tokenPtr);
        WC_GUARD(varDecl, nullptr);
        return new StmntVarDecl(*varDecl);
    }
    
    // Parse if statement if ahead
    if (IfStmnt::peek(tokenPtr)) {
        IfStmnt * ifStmnt = IfStmnt::parse(tokenPtr);
        WC_GUARD(ifStmnt, nullptr);
        return new StmntIfStmnt(*ifStmnt);
    }
    
    // Parse while statement if ahead
    if (WhileStmnt::peek(tokenPtr)) {
        WhileStmnt * whileStmnt = WhileStmnt::parse(tokenPtr);
        WC_GUARD(whileStmnt, nullptr);
        return new StmntWhileStmnt(*whileStmnt);
    }
    
    // Otherwise parse assign expression
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(assignExpr, nullptr);
    return new StmntAssignExpr(*assignExpr);
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

bool StmntNopStmnt::codegenStmnt(const CodegenCtx & cgCtx) {
    return mStmnt.codegenStmnt(cgCtx);
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

bool StmntPrintStmnt::codegenStmnt(const CodegenCtx & cgCtx) {
    return mStmnt.codegenStmnt(cgCtx);
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

bool StmntIfStmnt::codegenStmnt(const CodegenCtx & cgCtx) {
    return mIfStmnt.codegenStmnt(cgCtx);
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

bool StmntWhileStmnt::codegenStmnt(const CodegenCtx & cgCtx) {
    return mWhileStmnt.codegenStmnt(cgCtx);
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

bool StmntVarDecl::codegenStmnt(const CodegenCtx & cgCtx) {
    return mDecl.codegenStmnt(cgCtx);
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
    
bool StmntAssignExpr::codegenStmnt(const CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx) != nullptr;
}

WC_END_NAMESPACE
