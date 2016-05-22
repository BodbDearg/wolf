#include "WCExpr.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintStmnt.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------

bool Expr::peek(const Token * tokenPtr) {
    return  PrintStmnt::peek(tokenPtr) ||
            VarDecl::peek(tokenPtr) ||
            AssignExpr::peek(tokenPtr);
}
    
Expr * Expr::parse(const Token *& tokenPtr) {
    // Parse print expression if ahead
    if (PrintStmnt::peek(tokenPtr)) {
        PrintStmnt * printStmnt = PrintStmnt::parse(tokenPtr);
        WC_GUARD(printStmnt, nullptr);
        return new ExprPrintStmnt(*printStmnt);
    }
    
    // Parse var decl if ahead
    if (VarDecl::peek(tokenPtr)) {
        VarDecl * varDecl = VarDecl::parse(tokenPtr);
        WC_GUARD(varDecl, nullptr);
        return new ExprVarDecl(*varDecl);
    }
    
    // Otherwise parse assign expression
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(assignExpr, nullptr);
    return new ExprAssign(*assignExpr);
}

//-----------------------------------------------------------------------------
// ExprPrint
//-----------------------------------------------------------------------------

ExprPrintStmnt::ExprPrintStmnt(PrintStmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & ExprPrintStmnt::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & ExprPrintStmnt::getEndToken() const {
    return mStmnt.getEndToken();
}

llvm::Value * ExprPrintStmnt::generateCode(const CodegenCtx & cgCtx) {
    return mStmnt.generateCode(cgCtx);
}

//-----------------------------------------------------------------------------
// ExprVarDecl
//-----------------------------------------------------------------------------

ExprVarDecl::ExprVarDecl(VarDecl & decl) : mDecl(decl) {
    mDecl.mParent = this;
}

const Token & ExprVarDecl::getStartToken() const {
    return mDecl.getStartToken();
}

const Token & ExprVarDecl::getEndToken() const {
    return mDecl.getEndToken();
}

llvm::Value * ExprVarDecl::generateCode(const CodegenCtx & cgCtx) {
    return mDecl.generateCode(cgCtx);
}

//-----------------------------------------------------------------------------
// ExprAssign
//-----------------------------------------------------------------------------

ExprAssign::ExprAssign(AssignExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & ExprAssign::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ExprAssign::getEndToken() const {
    return mExpr.getEndToken();
}
    
llvm::Value * ExprAssign::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

WC_END_NAMESPACE
