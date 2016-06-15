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
    const DataValue * dataValue = lookupDataValue();
    
    if (dataValue) {
        WC_ASSERT(dataValue->type);
        return *dataValue->type;
    }

    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
}

llvm::Value * Identifier::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("Can't codegen address of variable '%s'! No such variable in current scope!", name());
        return nullptr;
    }
    
    // Return it!
    WC_ASSERT(dataValue->value);
    return dataValue->value;
}

llvm::Value * Identifier::codegenExprEval(CodegenCtx & cgCtx) {
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("No variable named '%s' in the current scope! Unable to take it's value!", name());
        return nullptr;
    }
    
    // TODO: this won't work for anything other than non primitives
    // Create an instruction to load it
    WC_ASSERT(dataValue->value);
    return cgCtx.irBuilder.CreateLoad(dataValue->value, std::string("load_ident_val:") + name());
}

const char * Identifier::name() const {
    return mToken.data.strVal.ptr;
}

const DataValue * Identifier::lookupDataValue() const {
    // Grab the parent scope, there should always be one
    const Scope * parentScope = getParentScope();
    WC_GUARD_ASSERT(parentScope, nullptr);
    
    // Grab the variable value from the parent scope
    return parentScope->getVariable(name());
}

WC_END_NAMESPACE
