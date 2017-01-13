#pragma once

#include "Lexer/WCToken.hpp"
#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class LinearAlloc;
struct Token;

/* Struct holding the current context for parsing */
struct ParseCtx {
    /* The current token we are on. Incremented as parsing happens. */
    const Token * curTok;
    
    /* A linear allocator that can be used during parsing. Used to allocate AST nodes */
    LinearAlloc & alloc;
    
    /* A list of error messages emitted during parsing */
    std::vector<std::string> errorMsgs;
    
    /* A list of warning messages emitted during parsing */
    std::vector<std::string> warningMsgs;
    
    /* Creates the parse context */
    ParseCtx(const Token * startToken, LinearAlloc & linearAlloc)
    :
        curTok(startToken),
        alloc(linearAlloc)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    /* Skip onto the next token */
    inline void nextTok() {
        ++curTok;
    }
    
    /* Skip 'N' tokens */
    inline void nextToks(size_t count) {
        curTok += count;
    }
};

WC_END_NAMESPACE
