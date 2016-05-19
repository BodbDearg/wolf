#include "WCIdentifier.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCScope.hpp"
#include "WCStringUtils.hpp"
#include "WCToken.hpp"
#include <memory>

WC_BEGIN_NAMESPACE

bool Identifier::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIdentifier;
}

Identifier * Identifier::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kIdentifier) {
        parseError(*tokenPtr, "Expected identifier!");
        return nullptr;
    }
    
    Identifier * intLit = new Identifier(*tokenPtr);
    ++tokenPtr;
    return intLit;
}

Identifier::Identifier(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & Identifier::getStartToken() const {
    return mToken;
}

const Token & Identifier::getEndToken() const {
    return mToken;
}

llvm::Value * Identifier::generateCode(const CodegenCtx & cgCtx) {
    // Grab the parent scope, there should always be one
    Scope * parentScope = getParentScope();
    WC_GUARD_ASSERT(parentScope, nullptr);
    
    // Grab the variable
    llvm::Value * value = parentScope->getVariable(mToken.data.strVal.ptr);
    
    if (!value) {
        std::unique_ptr<char[]> identifierNameUtf8(StringUtils::convertUtf32ToUtf8(mToken.data.strVal.ptr,
                                                                                   mToken.data.strVal.length));
        
        compileError("No variable named '%s' in the current scope!", identifierNameUtf8.get());
    }
    
    // Create an instruction to load it
    return cgCtx.irBuilder.CreateLoad(value);
}

llvm::Value * Identifier::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    Scope * parentScope = getParentScope();
    WC_GUARD_ASSERT(parentScope, nullptr);
    return parentScope->getVariable(mToken.data.strVal.ptr);
}

char * Identifier::getUtf8Name() const {
    return StringUtils::convertUtf32ToUtf8(mToken.data.strVal.ptr, mToken.data.strVal.length);
}

WC_END_NAMESPACE
