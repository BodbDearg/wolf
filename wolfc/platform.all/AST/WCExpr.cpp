#include "WCExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintExpr.hpp"

WC_BEGIN_NAMESPACE

bool Expr::peek(const Token * tokenPtr) {
    // TODO: peek for AssignExpr
    return PrintExpr::peek(tokenPtr);
}
    
Expr * Expr::parse(ASTNode & parent, const Token *& tokenPtr) {
    if (PrintExpr::peek(tokenPtr)) {
        PrintExpr * printExpr = PrintExpr::parse(parent, tokenPtr);
        WC_GUARD(printExpr, nullptr);
        return new ExprPrint(parent, *printExpr);
    }
    // TODO: peek and parse AssignExpr here
    else {
        error("Expected expression statement!");
    }
    
    return nullptr;
}

Expr::Expr(ASTNode & parent) : ASTNodeCodegen(parent) {
    
}

ExprPrint::ExprPrint(ASTNode & parent, PrintExpr & expr) :
    Expr(parent),
    mExpr(expr)
{
    WC_EMPTY_FUNC_BODY();
}
    
llvm::Value * ExprPrint::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}
    
ExprAssign::ExprAssign(ASTNode & parent, AssignExpr & expr) :
    Expr(parent),
    mExpr(expr)
{
    WC_EMPTY_FUNC_BODY();
}
    
llvm::Value * ExprAssign::generateCode(const CodegenCtx & cgCtx) {
    // TODO: implement
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

WC_END_NAMESPACE
