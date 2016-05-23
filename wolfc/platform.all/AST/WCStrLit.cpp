#include "WCStrLit.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStringUtils.hpp"
#include "WCToken.hpp"
#include <memory>

WC_BEGIN_NAMESPACE

bool StrLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kStrLit;
}

StrLit * StrLit::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kStrLit) {
        parseError(*tokenPtr, "Expected string literal!");
        return nullptr;
    }
    
    StrLit * intLit = new StrLit(*tokenPtr);
    ++tokenPtr;
    return intLit;
}

StrLit::StrLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & StrLit::getStartToken() const {
    return mToken;
}

const Token & StrLit::getEndToken() const {
    return mToken;
}

llvm::Value * StrLit::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.CreateGlobalStringPtr(mToken.data.strVal.ptr, "usr_string_lit");
}

WC_END_NAMESPACE
