#include "WCStrLit.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool StrLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kStrLit;
}

StrLit * StrLit::parse(const Token *& tokenPtr) {
    if (!peek(tokenPtr)) {
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
    // TODO: need some sort of LUT for the module so we only have unique string instances
    return cgCtx.irBuilder.CreateGlobalStringPtr(mToken.data.strVal.ptr, "usr_string_lit");
}

const DataType & StrLit::getDataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kStr);
}

WC_END_NAMESPACE
