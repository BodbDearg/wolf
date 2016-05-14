#include "WCScope.hpp"
#include "WCExprs.hpp"

WC_BEGIN_NAMESPACE

bool Scope::peek(const Token * tokenPtr) {
    return Exprs::peek(tokenPtr);
}

Scope * Scope::parse(const Token *& tokenPtr) {
    Exprs * exprs = Exprs::parse(tokenPtr);
    WC_GUARD(exprs, nullptr);
    return new Scope(*exprs);
}

Scope::Scope(Exprs & exprs) : mExprs(exprs) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * Scope::generateCode(const CodegenCtx & cgCtx) {
    return mExprs.generateCode(cgCtx);
}

WC_END_NAMESPACE
