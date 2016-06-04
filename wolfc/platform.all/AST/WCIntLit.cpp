#include "WCIntLit.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool IntLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIntLit;
}

IntLit * IntLit::parse(const Token *& tokenPtr) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected integer literal!");
        return nullptr;
    }
    
    IntLit * intLit = new IntLit(*tokenPtr);
    ++tokenPtr;
    return intLit;
}

IntLit::IntLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & IntLit::getStartToken() const {
    return mToken;
}

const Token & IntLit::getEndToken() const {
    return mToken;
}

llvm::Value * IntLit::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt64(mToken.data.intVal);
}

const DataType & IntLit::getDataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt);
}

WC_END_NAMESPACE
