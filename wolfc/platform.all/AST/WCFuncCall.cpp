#include "WCFuncCall.hpp"
#include "WCFuncCallArgList.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

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
    
    // See if there is an arg list:
    FuncCallArgList * argList = nullptr;
    
    if (FuncCallArgList::peek(currentToken)) {
        argList = FuncCallArgList::parse(currentToken, alloc);
        WC_GUARD(argList, nullptr);
    }
    
    // Expect ')'
    if (currentToken->type != TokenType::kRParen) {
        parseError(*currentToken, "Expected ')' !");
        return nullptr;
    }
    
    // Save and skip ')'
    const Token * endToken = currentToken;
    ++currentToken;
    
    // No args:
    return WC_NEW_AST_NODE(alloc, FuncCall, *startToken, argList, *endToken);
}

FuncCall::FuncCall(const Token & startToken, FuncCallArgList * argList, const Token & endToken) :
    mStartToken(startToken),
    mArgList(argList),
    mEndToken(endToken)
{
    if (mArgList) {
        mArgList->mParent = this;
    }
}

const Token & FuncCall::getStartToken() const {
    return mStartToken;
}

const Token & FuncCall::getEndToken() const {
    return mEndToken;
}

WC_END_NAMESPACE
