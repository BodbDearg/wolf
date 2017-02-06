#include "Identifier.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/UnevalDataType.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool Identifier::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIdentifier;
}

Identifier * Identifier::parse(ParseCtx & parseCtx) {
    if (parseCtx.tok()->type != TokenType::kIdentifier) {
        parseCtx.error("Expected identifier!");
        return nullptr;
    }
    
    Identifier * intLit = WC_NEW_AST_NODE(parseCtx, Identifier, *parseCtx.tok());
    parseCtx.nextTok();
    return intLit;
}

Identifier::Identifier(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void Identifier::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
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

bool Identifier::isConstExpr() const {
    // FIXME: Implement this
    return false;
}

const DataType & Identifier::dataType() const {
    // We won't know this until further analysis at compile time
    return PrimitiveDataTypes::getUnevalDataType();
}

const char * Identifier::name() const {
    return mToken.data.strVal.ptr;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
