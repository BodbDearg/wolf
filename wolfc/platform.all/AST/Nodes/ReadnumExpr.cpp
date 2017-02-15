#include "ReadnumExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool ReadnumExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kReadnum;
}

ReadnumExpr * ReadnumExpr::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    if (parseCtx.tok()->type != TokenType::kReadnum) {
        parseCtx.error("Expected 'readnum' at begining of readnum() expression!");
        return nullptr;
    }
    
    const Token * readnumTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume 'readnum'
    
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expect '(' following 'readnum'!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Consume '('
    
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume ')'
    
    return WC_NEW_AST_NODE(parseCtx, ReadnumExpr, *readnumTok, *rparenTok);
}

ReadnumExpr::ReadnumExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void ReadnumExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ReadnumExpr::getStartToken() const {
    return mStartToken;
}

const Token & ReadnumExpr::getEndToken() const {
    return mEndToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
