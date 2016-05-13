#include "WCScope.hpp"
#include "WCPrintExprs.hpp"

WC_BEGIN_NAMESPACE

bool Scope::peek(const Token * tokenPtr) {
    return PrintExprs::peek(tokenPtr);
}

Scope * Scope::parse(const Token *& tokenPtr) {
    PrintExprs * printExprs = PrintExprs::parse(tokenPtr);
    
    if (!printExprs) {
        return nullptr;
    }
    
    return new Scope(*printExprs);
}

Scope::Scope(PrintExprs & exprs) : mExprs(exprs) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * Scope::generateCode(const CodegenCtx & cgCtx) {
    return mExprs.generateCode(cgCtx);
}

WC_END_NAMESPACE
