#include "WCFuncInvocation.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// FuncInvocation
//-----------------------------------------------------------------------------

bool FuncInvocation::peek(const Token * currentToken) {
    return currentToken->type == TokenType::kLParen;
}

FuncInvocation * FuncInvocation::parse(const Token *& currentToken, LinearAlloc & alloc) {
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
    return WC_NEW_AST_NODE(alloc, FuncInvocationNoArgs, *startToken, *endToken);
}

//-----------------------------------------------------------------------------
// FuncInvocationNoArgs
//-----------------------------------------------------------------------------

FuncInvocationNoArgs::FuncInvocationNoArgs(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & FuncInvocationNoArgs::getStartToken() const {
    return mStartToken;
}

const Token & FuncInvocationNoArgs::getEndToken() const {
    return mEndToken;
}

WC_END_NAMESPACE
