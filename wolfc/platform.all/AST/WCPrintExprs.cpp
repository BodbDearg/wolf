#include "WCPrintExprs.hpp"
#include "WCPrintExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"
#include "WCFinally.hpp"

WC_BEGIN_NAMESPACE

bool PrintExprs::peek(const Token * tokenPtr) {
    return PrintExpr::peek(tokenPtr);
}

PrintExprs * PrintExprs::parse(const Token *& tokenPtr) {
    PrintExpr * startExpr = PrintExpr::parse(tokenPtr);
    WC_GUARD(startExpr, nullptr);
    
    if (PrintExprs::peek(tokenPtr)) {
        PrintExprs * endExprs = PrintExprs::parse(tokenPtr);
        WC_GUARD(endExprs, nullptr);
        
        return new PrintExprsMulti(*startExpr, *endExprs);
    }
    else {
        return new PrintExprsSingle(*startExpr);
    }
}

PrintExprsSingle::PrintExprsSingle(PrintExpr & expr) : mExpr(expr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * PrintExprsSingle::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

PrintExprsMulti::PrintExprsMulti(PrintExpr & startExpr, PrintExprs & endExprs) :
    mStartExpr(startExpr),
    mEndExprs(endExprs)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * PrintExprsMulti::generateCode(const CodegenCtx & cgCtx) {
    // Generate the instructions for all these calls
    WC_GUARD(mStartExpr.generateCode(cgCtx), nullptr);
    WC_GUARD(mEndExprs.generateCode(cgCtx), nullptr);
    
    // Return the last instruction generate
    return &(*(cgCtx.irBuilder.GetInsertPoint()));
}

WC_END_NAMESPACE
