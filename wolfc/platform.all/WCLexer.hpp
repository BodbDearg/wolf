#pragma once

#include <cstddef>

class WCToken;

/**
 * Splits input source text into Wolf tokens.
 * See WCToken for more info on valid tokens in the language.
 */
class WCLexer {
public:
    /* The default token capacity for the lexer */
    static constexpr const size_t kDefaultInitialTokenCapacity = 1024 * 32;
    
    /**
     * Creates the lexer. The given number of tokens is allocated
     * for the initial token capacity.
     */
    WCLexer(size_t initialTokenCapacity = kDefaultInitialTokenCapacity);
    
    /**
     * Parse the given input source string and convert into a series of tokens.
     * Returns false and saves an error message if this process fails.
     */
    bool parse(const char32_t * srcText);
    
private:
    /* Increase the token list capacity to the given capacity. If smaller than old capacity, nothing happens. */
    void increaseTokenListCapacity(size_t newCapacity);
    
    /* Allocate a token from the token list. The token constructor is NOT called. */
    WCToken * allocToken();
    
    /* The list of parsed tokens */
    WCToken * mTokenList;
    
    /* The number of tokens currently allocated */
    size_t mTokenCapacity;
    
    /* The number of tokens actually in the token list */
    size_t mTokenCount;
};
