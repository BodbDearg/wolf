#include "WCStmnt.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintStmnt.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------

bool Stmnt::peek(const Token * tokenPtr) {
    return  PrintStmnt::peek(tokenPtr) ||
            VarDecl::peek(tokenPtr) ||
            AssignExpr::peek(tokenPtr);
}
    
Stmnt * Stmnt::parse(const Token *& tokenPtr) {
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
    
    // Otherwise parse assign expression
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(assignExpr, nullptr);
    return new StmntAssignExpr(*assignExpr);
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
