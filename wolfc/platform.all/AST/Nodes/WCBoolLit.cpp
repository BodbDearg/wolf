#include "WCBoolLit.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool BoolLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTrue || tokenPtr->type == TokenType::kFalse;
}

BoolLit * BoolLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected 'bool' literal!");
        return nullptr;
    }
    
    BoolLit * boolLit = WC_NEW_AST_NODE(parseCtx, BoolLit, *parseCtx.tok());
    parseCtx.nextTok();
    return boolLit;
}

BoolLit::BoolLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void BoolLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & BoolLit::getStartToken() const {
    return mToken;
}

const Token & BoolLit::getEndToken() const {
    return mToken;
}

bool BoolLit::isLValue() const {
    return false;
}

bool BoolLit::isConstExpr() const {
    return true;
}

const DataType & BoolLit::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
