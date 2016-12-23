#include "WCNoOpStmnt.hpp"

#include "Lexer/WCToken.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

bool NoOpStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNoOp;
}

NoOpStmnt * NoOpStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial 'noop'
    if (tokenPtr->type != TokenType::kNoOp) {
        parseError(*tokenPtr, "'noop' statement expected!");
        return nullptr;
    }
    
    // Consume and save the token and return the parsed statement
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    return WC_NEW_AST_NODE(alloc, NoOpStmnt, *startToken);
}

NoOpStmnt::NoOpStmnt(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NoOpStmnt::getStartToken() const {
    return mToken;
}

const Token & NoOpStmnt::getEndToken() const {
    return mToken;
}

bool NoOpStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

bool NoOpStmnt::codegen(CodegenCtx & cgCtx) {
    // Like it's name suggests 'noop' actually does nothing... Shock, horror!
    WC_UNUSED_PARAM(cgCtx);
    return true;
}

WC_END_NAMESPACE
