#include "WCNopStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NopStmnt
//-----------------------------------------------------------------------------

bool NopStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNop;
}

NopStmnt * NopStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial 'nop'
    if (tokenPtr->type != TokenType::kNop) {
        parseError(*tokenPtr, "'nop' statement expected!");
        return nullptr;
    }
    
    // Consume and save the token and return the parsed statement
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    return WC_NEW_AST_NODE(alloc, NopStmnt, *startToken);
}

NopStmnt::NopStmnt(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NopStmnt::getStartToken() const {
    return mToken;
}

const Token & NopStmnt::getEndToken() const {
    return mToken;
}

bool NopStmnt::codegen(CodegenCtx & cgCtx) {
    // Like it's name suggests 'nop' actually does nothing. Shock, horror!
    WC_UNUSED_PARAM(cgCtx);
    return true;
}

WC_END_NAMESPACE
