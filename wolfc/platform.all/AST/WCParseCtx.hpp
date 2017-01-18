#pragma once

#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
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

/* Class holding the current context for parsing */
class ParseCtx {
public:
    /* Creates the parse context */
    ParseCtx(const Token * startToken, LinearAlloc & linearAlloc);
    
    /**
     * Get the pointer to the current token we are on.
     * Note that subsequent tokens can be accessed using array indexing if lookahead is required, 
     * since the tokens are stored in a regular C style array.
     */
    inline const Token * tok() {
        return mCurrentToken;
    }
    
    /* Skip onto the next token. Will not move past the current token if we are on EOF */
    inline void nextTok() {
        if (mCurrentToken->type != TokenType::kEOF) {
            ++mCurrentToken;
        }
    }
    
    /* Skip 'N' tokens */
    inline void nextToks(size_t count) {
        while (count > 0) {
            nextTok();
            --count;
        }
    }
    
    /* Get the liner allocator for the parse context */
    inline LinearAlloc & getAlloc() {
        return mLinearAlloc;
    }
    
    /* Tells if there are errors in the parse context */
    bool hasErrors() const;
    
    /* Get the error messages stored in the parse context */
    inline const auto & getErrorMsgs() const {
        return mErrorMsgs;
    }
    
    /* Tells if there are warnings in the parse context */
    bool hasWarnings() const;
    
    /* Get the warning messages stored in the parse context */
    inline const auto & getWarningMsgs() const {
        return mWarningMsgs;
    }
    
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
    
private:
    /* The current token we are on. Incremented as parsing happens. */
    const Token * mCurrentToken;
    
    /* A linear allocator that can be used during parsing. Used to allocate AST nodes */
    LinearAlloc & mLinearAlloc;
    
    /* A list of error messages emitted during parsing */
    std::vector<std::string> mErrorMsgs;
    
    /* A list of warning messages emitted during parsing */
    std::vector<std::string> mWarningMsgs;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
