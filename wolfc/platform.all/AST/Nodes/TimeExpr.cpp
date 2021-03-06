//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "TimeExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool TimeExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTime;
}

TimeExpr * TimeExpr::parse(ParseCtx & parseCtx) {
    // Consume 'time' and skip any newlines that follow
    if (parseCtx.tok()->type != TokenType::kTime) {
        parseCtx.error("Expected 'time' at begining of time() expression!");
        return nullptr;
    }
    
    const Token * readnumTok = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the '(' and skip any newlines that follow
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expect '(' following 'time'!");
        return nullptr;
    }
    
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the closing ')' and return the parsed node
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = parseCtx.tok();
    parseCtx.nextTok();
    return WC_NEW_AST_NODE(parseCtx, TimeExpr, *readnumTok, *rparenTok);
}

TimeExpr::TimeExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    
}

void TimeExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TimeExpr::getStartToken() const {
    return mStartToken;
}

const Token & TimeExpr::getEndToken() const {
    return mEndToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
