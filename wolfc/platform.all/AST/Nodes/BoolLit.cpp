#include "BoolLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool BoolLit::peek(const Token * tokenPtr) {
    #warning Handle newlines during parsing
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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
