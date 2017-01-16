#pragma once

#include "Lexer/WCToken.hpp"
#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdarg>
    #include <string>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class LinearAlloc;
struct Token;

WC_AST_BEGIN_NAMESPACE

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
    
    /* Tells if there are errors in the parse context */
    bool hasErrors() const;
    
    /* Tells if there are warnings in the parse context */
    bool hasWarnings() const;
    
    /**
     * Emit an error message and save to the list of error messages in the parse context.
     *
     * If a token is given to emit the error message at, then the line and column info for the 
     * error will be at the given token; otherwise the current token in the parse context will be
     * used (typical usage).
     *
     * Emitting an error will put the parse context into an error state.
     */
    void error(const char * msgFmtStr, ...);
    void error(const Token & atToken, const char * msgFmtStr, ...);
    void error(const char * msgFmtStr, std::va_list msgFmtStrArgs);
    void error(const Token & atToken, const char * msgFmtStr, std::va_list msgFmtStrArgs);
    
    /**
     * Emit a warning message and save to the list of warning messages in the parse context.
     *
     * If a token is given to emit the warning message at, then the line and column info for the
     * warning will be at the given token; otherwise the current token in the parse context will be
     * used (typical usage).
     */
    void warning(const char * msgFmtStr, ...);
    void warning(const Token & atToken, const char * msgFmtStr, ...);
    void warning(const char * msgFmtStr, std::va_list msgFmtStrArgs);
    void warning(const Token & atToken, const char * msgFmtStr, std::va_list msgFmtStrArgs);
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
