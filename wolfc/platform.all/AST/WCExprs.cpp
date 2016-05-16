#include "WCExprs.hpp"
#include "WCCodegenCtx.hpp"
#include "WCExpr.hpp"

WC_BEGIN_NAMESPACE

bool Exprs::peek(const Token * tokenPtr) {
    return Expr::peek(tokenPtr);
}
    
Exprs * Exprs::parse(ASTNode & parent, const Token *& tokenPtr) {
    Expr * expr = Expr::parse(parent, tokenPtr);
    WC_GUARD(expr, nullptr);
    
    if (Exprs::peek(tokenPtr)) {
        Exprs * exprs = Exprs::parse(parent, tokenPtr);
        WC_GUARD(exprs, nullptr);
        return new ExprsMulti(parent, *expr, *exprs);
    }
    else {
        return new ExprsSingle(parent, *expr);
    }
}

Exprs::Exprs(ASTNode & parent) : ASTNodeCodegen(parent) {
    WC_EMPTY_FUNC_BODY();
}

ExprsSingle::ExprsSingle(ASTNode & parent, Expr & expr) :
    Exprs(parent),
    mExpr(expr)
{
    WC_EMPTY_FUNC_BODY();
}
    
llvm::Value * ExprsSingle::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
};
    
ExprsMulti::ExprsMulti(ASTNode & parent, Expr & leftExpr, Exprs & rightExprs) :
    Exprs(parent),
    mLeftExpr(leftExpr),
    mRightExprs(rightExprs)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * ExprsMulti::generateCode(const CodegenCtx & cgCtx) {
    // Generate the instructions for all these calls
    WC_GUARD(mLeftExpr.generateCode(cgCtx), nullptr);
    WC_GUARD(mRightExprs.generateCode(cgCtx), nullptr);
    
    // Return the last instruction generated
    return &(*(cgCtx.irBuilder.GetInsertPoint()));
}

WC_END_NAMESPACE
