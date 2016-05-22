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

llvm::Value * Scope::generateCode(const CodegenCtx & cgCtx) {
    return mStmnts.generateCode(cgCtx);
}

llvm::Value * Scope::getOrCreateVariable(const char32_t * variableName,
                                         const CodegenCtx & cgCtx)
{
    llvm::Value *& val = mVariableValues[variableName];
    
    if (!val) {
        std::unique_ptr<char[]> varNameUtf8(StringUtils::convertUtf32ToUtf8(variableName,
                                                                            StringUtils::strlen(variableName)));
        
        val = cgCtx.irBuilder.CreateAlloca(llvm::Type::getInt64Ty(cgCtx.llvmCtx),
                                           nullptr,
                                           std::string("alloc_ident_val:") + varNameUtf8.get());
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
