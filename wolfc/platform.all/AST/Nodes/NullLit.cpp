//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "NullLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool NullLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNull;
}

NullLit * NullLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected 'null' literal!");
        return nullptr;
    }
    
    NullLit * nullLit = WC_NEW_AST_NODE(parseCtx, NullLit, *parseCtx.tok());
    parseCtx.nextTok();
    return nullLit;
}

NullLit::NullLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void NullLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NullLit::getStartToken() const {
    return mToken;
}

const Token & NullLit::getEndToken() const {
    return mToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
