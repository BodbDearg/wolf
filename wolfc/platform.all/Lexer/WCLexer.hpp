#pragma once

#include "WCTokenType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct Token;

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
     * Parse the given input source string (encoded in UTF8 format) and convert into a series of tokens.
     * Returns false and saves an error message if this process fails.
     */
    bool process(const char * utf8Src);
    
    /* Give the token list for the lexer */
    const Token * getTokenList() const;
    
private:
    /* Struct holding lexer state, current source pointer and line/col info */
    struct LexerState {
        bool error;
        const char * srcPtr;
        size_t srcLine;
        size_t srcCol;
        char32_t currentChar;
        size_t currentCharNumBytes;
        bool currentCharIsNewline;
    };
    
    /* Result of parsing */
    enum class ParseResult {
        kNone,      /* Didn't parse anything */
        kSuccess,   /* Parsing went ok */
        kFail       /* Parsing failed */
    };
    
    /* Initialize the lexer state. */
    bool initLexerState(const char * utf8Src);
    
    /* Try to consume some whitespace. Return false if no whitespace was consumed. */
    bool trySkipWhitespaceChar();

    /* Attempt to skip multiple whitespace chars ahead. Stops when no more are found. */
    void skipAnyWhitespaceCharsAhead();
    
    /* Move onto the next character. */
    bool moveOntoNextChar();
    
    /* Try to parse some basic tokens. */
    ParseResult parseBasicTokens();
    
    /**
     * Create a basic token (symbol etc.) of the given type and add it to the end of the token list.
     * The given number of characters are consumed. If the parse fails then that is reflected in the return result.
     */
    ParseResult parseBasicToken(TokenType tokenType, size_t numCharsInToken);
    
    /* Try to parse a numeric literal. Return false if no numeric literal was parsed. */
    ParseResult parseNumericLiteral();
    
    /* Try to parse a double quoted string literal. Return false if no string literal was parsed. */
    ParseResult parseDoubleQuotedStringLiteral();
    
    /* Try to parse keywords and literals. */
    ParseResult parseKeywordsAndLiterals();
    
    /* Parse single line comments */
    ParseResult parseSingleLineComments();
    
    /* Parse multi line comments */
    ParseResult parseMultiLineComments();
    
    /* Increase the token list capacity to the given capacity. If smaller than old capacity, nothing happens. */
    void increaseTokenListCapacity(size_t newCapacity);
    
    /**
     * Allocate a token from the token list. 
     * All fields in the token are default initialized apart from token type.
     * It is up to the callee to perform any other initialization that is required.
     */
    Token & allocToken(TokenType tokenType);

    /* Pop the top token off the token list. Expects that there is at least one token allocated. */
    void popToken();
    
    /* Create the EOF token and add it to the end of the token list */
    void createEOFToken();

    /**
     * Emit a lexer error to stderror followed by a newline.
     * The given line and column information are emitted also.
     */
    void error(size_t srcLine, size_t srcCol, const char * msg, ...);
    
    /**
     * Emit a lexer error to stderror follwed by a newline.
     * The current lexer state line and column information are emitted also. 
     */
    void error(const char * msg, ...);
    
    /* The list of parsed tokens */
    Token * mTokenList;
    
    /* The number of tokens currently allocated */
    size_t mTokenCapacity;
    
    /* The number of tokens actually in the token list */
    size_t mTokenCount;
    
    /* The current state of the lexer */
    LexerState mLexerState;
    
#if DEBUG == 1
    /* A list for ease of debugging/inspection not used for release builds - list of parsed tokens */
    std::vector<Token*> mDebugTokenList;
#endif  // #if DEBUG == 1
};

WC_END_NAMESPACE
