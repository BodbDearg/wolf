#include "WCUnaryExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool UnaryExpr::peek(const Token * currentToken) {
    if (currentToken->type == TokenType::kIntLiteral ||
        currentToken->type == TokenType::kMinus ||
        currentToken->type == TokenType::kPlus ||
        currentToken->type == TokenType::kLParen)
    {
        return true;
    }
    
    return false;
}

UnaryExpr * UnaryExpr::parse(const Token *& currentToken) {
    #error TODO
}

WC_END_NAMESPACE
