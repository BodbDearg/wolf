#include "WCScope.hpp"
#include "WCCodegenCtx.hpp"
#include "WCExprs.hpp"

WC_BEGIN_NAMESPACE

bool Scope::peek(const Token * tokenPtr) {
    return Exprs::peek(tokenPtr);
}

Scope * Scope::parse(ASTNode & parent, const Token *& tokenPtr) {
    Exprs * exprs = Exprs::parse(parent, tokenPtr);
    WC_GUARD(exprs, nullptr);
    return new Scope(parent, *exprs);
}

Scope::Scope(ASTNode & parent, Exprs & exprs) :
    ASTNodeCodegen(parent),
    mExprs(exprs)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * Scope::generateCode(const CodegenCtx & cgCtx) {
    return mExprs.generateCode(cgCtx);
}

llvm::Value * Scope::getOrCreateVariable(const char32_t * variableName,
                                         const CodegenCtx & cgCtx)
{
    llvm::Value *& val = mVariableValues[variableName];
    
    if (!val) {
        val = cgCtx.irBuilder.CreateAlloca(llvm::Type::getInt64Ty(cgCtx.llvmCtx));
    }
    
    return val;
}

llvm::Value * Scope::getVariable(const char32_t * variableName) const {
    auto iter = mVariableValues.find(variableName);
    
    if (iter != mVariableValues.end()) {
        return iter->second;
    }
    
    return nullptr;
}

WC_END_NAMESPACE
