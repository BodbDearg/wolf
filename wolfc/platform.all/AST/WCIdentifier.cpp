#include "WCIdentifier.hpp"
#include "WCCodegenCtx.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool Identifier::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIdentifier;
}

Identifier * Identifier::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kIdentifier) {
        error(*tokenPtr, "Expected identifier!");
        return nullptr;
    }
    
    Identifier * intLit = new Identifier(*tokenPtr);
    ++tokenPtr;
    return intLit;
}

Identifier::Identifier(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * Identifier::generateCode(const CodegenCtx & cgCtx) {
    Scope * parentScope = getParentScope();
    
    if (!parentScope) {
        error("Can't codegen, no parent scope!");
        return nullptr;
    }
    
    return parentScope->getOrCreateVariable(mToken.data.strVal.ptr, cgCtx);
}

WC_END_NAMESPACE
