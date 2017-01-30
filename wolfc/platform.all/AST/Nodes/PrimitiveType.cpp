#include "PrimitiveType.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool PrimitiveType::peek(const Token * currentToken) {
    return PrimitiveDataTypes::getUsingLangKeyword(currentToken->type).isValid();
}

PrimitiveType * PrimitiveType::parse(ParseCtx & parseCtx) {
    // Must be a valid token ahead
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected primitive data type!");
        return nullptr;
    }
    
    // Save the token and skip
    const Token * token = parseCtx.tok();
    parseCtx.nextTok();

    // Return the node:
    return WC_NEW_AST_NODE(parseCtx, PrimitiveType, *token);
}

PrimitiveType::PrimitiveType(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void PrimitiveType::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimitiveType::getStartToken() const {
    return mToken;
}

const Token & PrimitiveType::getEndToken() const {
    return mToken;
}

const DataType & PrimitiveType::dataType() const {
    return PrimitiveDataTypes::getUsingLangKeyword(mToken.type);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE