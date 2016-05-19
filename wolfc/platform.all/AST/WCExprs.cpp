#include "WCExprs.hpp"
#include "WCCodegenCtx.hpp"
#include "WCExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Exprs
//-----------------------------------------------------------------------------

bool Exprs::peek(const Token * tokenPtr) {
    return Expr::peek(tokenPtr);
}
    
Exprs * Exprs::parse(const Token *& tokenPtr) {
    const Token * startTok = tokenPtr;
    Expr * expr = Expr::parse(tokenPtr);
    WC_GUARD(expr, nullptr);
    
    if (Exprs::peek(tokenPtr)) {
        if (tokenPtr->srcLine <= startTok->srcLine) {
            // TODO: support semi colon for multiple expressions per line
            parseError(*tokenPtr, "Multiple expressions are not allowed on one line!");
            return nullptr;
        }
        
        Exprs * exprs = Exprs::parse(tokenPtr);
        WC_GUARD(exprs, nullptr);
        return new ExprsMulti(*expr, *exprs);
    }
    else {
        return new ExprsSingle(*expr);
    }
}

//-----------------------------------------------------------------------------
// ExprsSingle
//-----------------------------------------------------------------------------

ExprsSingle::ExprsSingle(Expr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & ExprsSingle::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ExprsSingle::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * ExprsSingle::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
};

//-----------------------------------------------------------------------------
// ExprsMulti
//-----------------------------------------------------------------------------

ExprsMulti::ExprsMulti(Expr & leftExpr, Exprs & rightExprs) :
    mLeftExpr(leftExpr),
    mRightExprs(rightExprs)
{
    mLeftExpr.mParent = this;
    mRightExprs.mParent = this;
}

const Token & ExprsMulti::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & ExprsMulti::getEndToken() const {
    return mRightExprs.getEndToken();
}

llvm::Value * ExprsMulti::generateCode(const CodegenCtx & cgCtx) {
    // Generate the instructions for all these calls
    WC_GUARD(mLeftExpr.generateCode(cgCtx), nullptr);
    WC_GUARD(mRightExprs.generateCode(cgCtx), nullptr);
    
    // Return the last instruction generated
    return &(*(cgCtx.irBuilder.GetInsertPoint()));
}

WC_END_NAMESPACE
