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

bool IntLit::isLValue() const {
    return false;
}

const DataType & IntLit::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt);
}

llvm::Value * IntLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * IntLit::codegenExprEval(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt64(mToken.data.intVal);
}

WC_END_NAMESPACE
