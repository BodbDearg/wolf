#pragma once

#include "WCToken.hpp"
#include <cstddef>
#include <vector>

WC_BEGIN_NAMESPACE

/**
 * Splits input source text into Wolf tokens.
 * See WCToken for more info on valid tokens in the language.
 */
class Lexer {
public:
    /* The default token capacity for the lexer */
    static constexpr const size_t kDefaultInitialTokenCapacity = 1024 * 32;
    
    /**
     * Creates the lexer. The given number of tokens is allocated
     * for the initial token capacity.
     */
    Lexer(size_t initialTokenCapacity = kDefaultInitialTokenCapacity);
    
    ~Lexer();
    
    /**
     * Parse the given input source string and convert into a series of tokens.
     * Returns false and saves an error message if this process fails.
     */
    bool process(const char32_t * srcText);
    
private:
    /* Try to consume some whitespace. Return false if no whitespace was consumed. */
    bool consumeWhitespace(char32_t currentChar);
    
    /**
     * Advance the lexer position by the given number of NON whitespace chars.
     *
     * Makes the following assumptions:
     *  (1) The given number of characters exist in the source code text.
     *  (2) The given number of characters are NOT whitespace, and treats them as so.
     */
    void consumeNonWhiteSpace(size_t numChars);
    
    /* Try to parse a numeric literal. Return false if no numeric literal was parsed. */
    bool parseNumericLiteral(char32_t currentChar);
    
    /* Try to parse some basic tokens. */
    bool parseBasicTokens(char32_t currentChar);
    
    /* Increase the token list capacity to the given capacity. If smaller than old capacity, nothing happens. */
    void increaseTokenListCapacity(size_t newCapacity);
    
    /**
     * Allocate a token from the token list. 
     * All fields in the token are initialized, source location info set and the type of the token
     * set also. If the token is a literal, it is up to the callee to perform any additional 
     * initialization that is required.
     */
    Token & allocToken(TokenType tokenType);
    
    /* Emit a lexer error to stderror. Line and column information are emitted also. */
    void error(const char * msg, ...);
    
    /* The list of parsed tokens */
    Token * mTokenList;
    
    /* The number of tokens currently allocated */
    size_t mTokenCapacity;
    
    /* The number of tokens actually in the token list */
    size_t mTokenCount;
    
    /* Current lexer state */
    struct LexerState {
        const char32_t * srcPtr;
        size_t srcLine;
        size_t srcCol;
    };
    
    LexerState mLexerState;
    
#if DEBUG == 1
    /* A list for ease of debugging/inspection not used for release builds - list of parsed tokens */
    std::vector<Token*> mDebugTokenList;
#endif  // #if DEBUG == 1
};

WC_END_NAMESPACE
