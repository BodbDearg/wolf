#include "WCFuncCall.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// FuncCall
//-----------------------------------------------------------------------------

bool FuncCall::peek(const Token * currentToken) {
    return currentToken->type == TokenType::kLParen;
}

FuncCall * FuncCall::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // Must open with '('
    if (currentToken->type != TokenType::kLParen) {
        parseError(*currentToken, "Expected '(' !");
        return nullptr;
    }
    
    // Save and skip '('
    const Token * startToken = currentToken;
    ++currentToken;
    
    // Expect '('
    if (currentToken->type != TokenType::kRParen) {
        parseError(*currentToken, "Expected ')' !");
        return nullptr;
    }
    
    // Save and skip ')'
    const Token * endToken = currentToken;
    ++currentToken;
    
    // TODO: function calls with args
    
    // No args:
    return WC_NEW_AST_NODE(alloc, FuncCallNoArgs, *startToken, *endToken);
}

//-----------------------------------------------------------------------------
// FuncCallNoArgs
//-----------------------------------------------------------------------------

FuncCallNoArgs::FuncCallNoArgs(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & FuncCallNoArgs::getStartToken() const {
    return mStartToken;
}

const Token & FuncCallNoArgs::getEndToken() const {
    return mEndToken;
}

WC_END_NAMESPACE
