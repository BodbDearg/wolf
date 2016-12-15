#include "WCPrimitiveType.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

bool PrimitiveType::peek(const Token * currentToken) {
    return PrimitiveDataTypes::getUsingLangKeyword(currentToken->type).isValid();
}

PrimitiveType * PrimitiveType::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // Must be a valid token ahead
    if (!peek(currentToken)) {
        parseError(*currentToken, "Expected primitive data type!");
        return nullptr;
    }
    
    // Save the token and skip
    const Token * token = currentToken;
    ++currentToken;

    // Return the node:
    return WC_NEW_AST_NODE(alloc, PrimitiveType, *token);
}

PrimitiveType::PrimitiveType(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & PrimitiveType::getStartToken() const {
    return mToken;
}

const Token & PrimitiveType::getEndToken() const {
    return mToken;
}

DataType & PrimitiveType::dataType() {
    return PrimitiveDataTypes::getUsingLangKeyword(mToken.type);
}

bool PrimitiveType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    return dataType().codegenLLVMTypeIfRequired(cgCtx, callingNode);
}

WC_END_NAMESPACE
