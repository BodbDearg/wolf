//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "StrLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool StrLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kStrLit;
}

StrLit * StrLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected string literal!");
        return nullptr;
    }
    
    StrLit * intLit = WC_NEW_AST_NODE(parseCtx, StrLit, *parseCtx.tok());
    parseCtx.nextTok();
    return intLit;
}

StrLit::StrLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void StrLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StrLit::getStartToken() const {
    return mToken;
}

const Token & StrLit::getEndToken() const {
    return mToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
