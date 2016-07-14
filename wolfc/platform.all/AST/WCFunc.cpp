#include "WCFunc.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool Func::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Must be a function ahead:
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the function identifier:
    Identifier * identifier = Identifier::parse(tokenPtr, alloc);
    WC_GUARD(identifier, nullptr);
    
    // Expect '('
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "'(' expected following function name!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip '('
    
    // Expect ')'
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "')' expected to close args list of function!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip ')'
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(tokenPtr, alloc);
    WC_GUARD(scope, nullptr);
    
    // Must be terminated by an 'end' token
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate function definition!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = tokenPtr;
    ++tokenPtr;
    
    // Done: return the parsed function
    return WC_NEW_AST_NODE(alloc, Func, *startToken, *identifier, *scope, *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mScope(scope),
    mEndToken(endToken)
{
    mIdentifier.mParent = this;
    mScope.mParent = this;
}

const Token & Func::getStartToken() const {
    return mStartToken;
}

const Token & Func::getEndToken() const {
    return mEndToken;
}

bool Func::codegen(CodegenCtx & cgCtx) {
    // TODO
    return false;
}

WC_END_NAMESPACE
