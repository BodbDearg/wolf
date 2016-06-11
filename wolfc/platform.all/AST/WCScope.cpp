#include "WCScope.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStmnts.hpp"
#include "WCStringUtils.hpp"

WC_BEGIN_NAMESPACE

bool Scope::peek(const Token * tokenPtr) {
    return Stmnts::peek(tokenPtr);
}

Scope * Scope::parse(const Token *& tokenPtr) {
    Stmnts * stmnts = Stmnts::parse(tokenPtr);
    WC_GUARD(stmnts, nullptr);
    return new Scope(*stmnts);
}

Scope::Scope(Stmnts & stmnts) : mStmnts(stmnts) {
    mStmnts.mParent = this;
}

const Token & Scope::getStartToken() const {
    return mStmnts.getStartToken();
}

const Token & Scope::getEndToken() const {
    return mStmnts.getEndToken();
}

bool Scope::codegenStmnt(CodegenCtx & cgCtx) {
    return mStmnts.codegenStmnt(cgCtx);
}

llvm::Value * Scope::getOrCreateVariable(const char * variableName,
                                         CodegenCtx & cgCtx)
{
    llvm::Value *& val = mVariableValues[variableName];
    
    if (!val) {
        val = cgCtx.irBuilder.CreateAlloca(llvm::Type::getInt64Ty(cgCtx.llvmCtx),
                                           nullptr,
                                           std::string("alloc_ident_val:") + variableName);
    }
    
    return val;
}

llvm::Value * Scope::getVariable(const char * variableName) const {
    // First search in this scope:
    auto iter = mVariableValues.find(variableName);
    
    if (iter != mVariableValues.end()) {
        return iter->second;
    }
    
    // If that fails try a parent scope:
    const Scope * parentScope = getParentScope();
    
    if (parentScope) {
        return parentScope->getVariable(variableName);
    }
    
    return nullptr;
}

WC_END_NAMESPACE
