#include "WCIdentifier.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

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

bool Identifier::isLValue() const {
    return true;
}

const DataType & Identifier::dataType() const {
    // TODO: support types other than 'int'
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt);
}

llvm::Value * Identifier::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    Scope * parentScope = getParentScope();
    WC_GUARD_ASSERT(parentScope, nullptr);
    return parentScope->getVariable(mToken.data.strVal.ptr);
}

llvm::Value * Identifier::codegenExprEval(CodegenCtx & cgCtx) {
    // Grab the parent scope, there should always be one
    Scope * parentScope = getParentScope();
    WC_GUARD_ASSERT(parentScope, nullptr);
    
    // Grab the variable
    llvm::Value * value = parentScope->getVariable(mToken.data.strVal.ptr);
    
    if (!value) {
        compileError("No variable named '%s' in the current scope!", mToken.data.strVal.ptr);
        return nullptr;
    }
    
    // Create an instruction to load it
    return cgCtx.irBuilder.CreateLoad(value, std::string("load_ident_val:") + mToken.data.strVal.ptr);
}

WC_END_NAMESPACE
