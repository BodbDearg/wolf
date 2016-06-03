#include "WCBoolLit.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool BoolLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTrue || tokenPtr->type == TokenType::kFalse;
}

BoolLit * BoolLit::parse(const Token *& tokenPtr) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected bool literal!");
        return nullptr;
    }
    
    BoolLit * boolLit = new BoolLit(*tokenPtr);
    ++tokenPtr;
    return boolLit;
}

BoolLit::BoolLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & BoolLit::getStartToken() const {
    return mToken;
}

const Token & BoolLit::getEndToken() const {
    return mToken;
}

llvm::Value * BoolLit::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt1(mToken.type == TokenType::kTrue);
}

WC_END_NAMESPACE
