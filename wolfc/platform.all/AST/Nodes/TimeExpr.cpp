#include "TimeExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/DataTypeId.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool TimeExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTime;
}

TimeExpr * TimeExpr::parse(ParseCtx & parseCtx) {
    if (parseCtx.tok()->type != TokenType::kTime) {
        parseCtx.error("Expected 'time' at begining of time() expression!");
        return nullptr;
    }
    
    const Token * readnumTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume 'time'
    
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expect '(' following 'time'!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Consume '('
    
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume ')'
    
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
