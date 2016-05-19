#include "WCLexer.hpp"
#include "WCAssert.hpp"
#include "WCCharUtils.hpp"
#include "WCStringUtils.hpp"
#include "WCToken.hpp"
#include <memory>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

WC_BEGIN_NAMESPACE

Lexer::Lexer(size_t initialTokenCapacity) :
    mTokenList(nullptr),
    mTokenCapacity(initialTokenCapacity),
    mTokenCount(0),
    mLexerState{}
{
    increaseTokenListCapacity(initialTokenCapacity);
}

Lexer::~Lexer() {
    if (mTokenList) {
        std::free(mTokenList);
        mTokenList = nullptr;
    }
}

bool Lexer::process(const char32_t * srcText) {
    // Init lexer state
    WC_ASSERT(srcText);
    mLexerState = {};
    mLexerState.srcPtr = srcText;
    
    // Continue until there is no source left
    while (char32_t c = mLexerState.srcPtr[0]) {
        // Skip all whitespace before parsing something interesting
        if (tryConsumeWhitespaceChar(c)) {
            continue;
        }
        
        // Try to parse stuff in this order of importance
        #define INVOKE_PARSE_FUNC(func)\
            \
            {\
                ParseResult result = func(c);\
                \
                if (result == ParseResult::kSuccess) {\
                    /* Parsing succeeded */\
                    continue;\
                }\
                else if (result != ParseResult::kNone) {\
                    /* Parsing failed */\
                    return false;\
                }\
            }
        
        INVOKE_PARSE_FUNC(parseBasicTokens);
        INVOKE_PARSE_FUNC(parseNumericLiteral);
        INVOKE_PARSE_FUNC(parseDoubleQuotedStringLiteral);
        INVOKE_PARSE_FUNC(parseKeywordsAndLiterals);
                                               
        // If we get to here then we have an error
        std::unique_ptr<char[]> charAsUtf8(StringUtils::convertUtf32ToUtf8(&c, 1));
        error("Unexpected character '%s' at start of token!", charAsUtf8.get());
        return false;
    }
    
    // Now add in the end of file token and return true for success
    allocToken(TokenType::kEOF, 0);
    return true;
}

const Token * Lexer::getTokenList() const {
    return mTokenList;
}

bool Lexer::tryConsumeWhitespaceChar(char32_t currentChar) {
    // Must be a whitespace char to consume at least one!
    WC_GUARD(CharUtils::isWhitespace(currentChar), false);
    
    // See if line separator or just normal whitespace
    if (CharUtils::isLineSeparator(currentChar)) {
        if (currentChar == '\r' && mLexerState.srcPtr[1] == '\n') {
            // Carriage return and newline combo: interpret as just one newline
            mLexerState.srcPtr += 2;
            mLexerState.srcLine += 1;
            mLexerState.srcCol = 0;
        }
        else {
            // Regular new line
            mLexerState.srcPtr += 1;
            mLexerState.srcLine += 1;
            mLexerState.srcCol = 0;
        }
    }
    else {
        // Regular white space
        mLexerState.srcPtr += 1;
        mLexerState.srcCol += 1;
    }
    
    return true;    // Parsed some whitespace
}

void Lexer::consumeNumNonWhiteSpaceChars(size_t numChars) {
    mLexerState.srcPtr += numChars;
    mLexerState.srcCol += numChars;
}

Lexer::ParseResult Lexer::parseBasicTokens(char32_t currentChar) {
    switch (currentChar) {
        case '(':
            allocToken(TokenType::kLParen, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case ')':
            allocToken(TokenType::kRParen, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case '+':
            allocToken(TokenType::kPlus, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case '-':
            allocToken(TokenType::kMinus, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case '*':
            allocToken(TokenType::kAsterisk, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case '/':
            allocToken(TokenType::kSlash, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        case '=':
            allocToken(TokenType::kEquals, 1);
            consumeNumNonWhiteSpaceChars(1);
            return ParseResult::kSuccess;
            
        default:
            break;
    }
    
    return ParseResult::kNone;
}

Lexer::ParseResult Lexer::parseNumericLiteral(char32_t currentChar) {
    // Must start with a digit to be a numeric
    WC_GUARD(CharUtils::isDigit(currentChar), ParseResult::kNone);
    
    // Continue until the end of the numeric literal
    const char32_t * tokStart = mLexerState.srcPtr;
    const char32_t * tokEnd = tokStart + 1;
    
    while ((currentChar = tokEnd[0])) {
        if (CharUtils::isDigit(currentChar)) {
            ++tokEnd;
        }
        else {
            break;
        }
    }
    
    // Skip these in the parser
    size_t numNumericChars = static_cast<size_t>(tokEnd - tokStart);
    consumeNumNonWhiteSpaceChars(numNumericChars);
    
    // Parse the number itself
    const char32_t * currentCharPtr = tokStart;
    uint64_t value = 0;
    
    while (currentCharPtr != tokEnd) {
        // TODO: check for numeric overflow
        uint64_t digit = currentCharPtr[0] - '0';
        value = value * 10 + digit;
        ++currentCharPtr;
    }
    
    // Now make the token and finish up
    Token & token = allocToken(TokenType::kIntLit, static_cast<size_t>(tokEnd - tokStart));
    token.data.intVal = value;
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseDoubleQuotedStringLiteral(char32_t currentChar) {
    // Must start with a double quote
    WC_GUARD(currentChar == '"', ParseResult::kNone);
    
    // Alright skip the opening '"'
    const char32_t * tokStart = mLexerState.srcPtr;
    consumeNumNonWhiteSpaceChars(1);
    const char32_t * strStart = mLexerState.srcPtr;
    
    // Save the lexer state at this point for later decoding:
    LexerState decodeLexState = mLexerState;
    
    // Search for the end of the string until we reach '"':
    char32_t c = mLexerState.srcPtr[0];
    
    while (c != '"') {
        // Bail with an error if we hit null
        if (c == 0) {
            error("Unexpected EOF while parsing double quoted string! Is the closing '\"' present?");
            return ParseResult::kFail;
        }
        
        // If we hit a newline then that is an error also
        if (CharUtils::isLineSeparator(c)) {
            error("Unexpected newline while parsing double quoted string! Newline characters must be escaped!");
            return ParseResult::kFail;
        }
        
        // Consume the char and move onto the next
        consumeNumNonWhiteSpaceChars(1);
        c = mLexerState.srcPtr[0];
    }
    
    // Now we have the end of the string
    const char32_t * strEnd = mLexerState.srcPtr;
    
    // Skip the end '"'
    consumeNumNonWhiteSpaceChars(1);
    const char32_t * tokEnd = mLexerState.srcPtr;
    
    // Compute the size of the buffer we need to hold the string in:
    size_t strBufferSize = static_cast<size_t>(strEnd - strStart + 1);
    
    // Allocate a token and the buffer to hold the string
    // TODO: what manages this memory?
    Token & tok = allocToken(TokenType::kStrLit, size_t(tokEnd - tokStart));
    tok.data.strVal.ptr = new char32_t[strBufferSize];
    char32_t * decodedStrPtr = tok.data.strVal.ptr;
    
    // Now decode the string into the buffer
    for (const char32_t * strCur = strStart; strCur != strEnd; ++strCur) {
        // See if escaped char:
        char32_t c1 = strCur[0];
        
        if (c1 == '\\') {
            // Possible escaped char: must not be at the end of the string though
            if (strCur + 1 >= strEnd) {
                error(decodeLexState, "Unexpected end string while parsing '\\' escape sequence!");
                return ParseResult::kFail;
            }
            
            // Read ahead and see what char we have
            char32_t c2 = strCur[1];
            
            // Now parse the escaped char
            char32_t escapedChar = 0;
            
            switch (c2) {
                case '"': escapedChar = '"'; break;
                case '\'': escapedChar = '\''; break;
                case '0': escapedChar = '\0'; break;
                case 'a': escapedChar = '\a'; break;
                case 'b': escapedChar = '\b'; break;
                case 'f': escapedChar = '\f'; break;
                case 'n': escapedChar = '\n'; break;
                case 'r': escapedChar = '\r'; break;
                case 't': escapedChar = '\t'; break;
                case 'v': escapedChar = '\v'; break;
                case '\\': escapedChar = '\\'; break;
                    
                default:
                    break;
            }
            
            // If it was an unrecognized char then spit out an error
            if (!escapedChar) {
                std::unique_ptr<char[]> escapedCharAsUtf8(StringUtils::convertUtf32ToUtf8(&c2, 1));
                error(decodeLexState, "Unrecognized escape sequence '\\%s'!", escapedCharAsUtf8.get());
                return ParseResult::kFail;
            }
            
            // Save the escaped char. Note: Need to skip two characters for this
            decodedStrPtr[0] = escapedChar;
            ++strCur;
            decodeLexState.srcPtr += 2;
            decodeLexState.srcCol += 2;
        }
        else {
            // Ordinary char, parse as normal
            decodedStrPtr[0] = c1;
            ++decodeLexState.srcPtr;
            ++decodeLexState.srcCol;
        }
        
        // Move along in the decoded string buffer
        ++decodedStrPtr;
        ++tok.data.strVal.length;
    }
    
    // Null terminate the decoded string when done
    tok.data.strVal.ptr[tok.data.strVal.length] = 0;
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseKeywordsAndLiterals(char32_t currentChar) {
    // Check start character is ok
    WC_GUARD(CharUtils::isValidIdentifierStartChar(currentChar), Lexer::ParseResult::kNone);
    
    // Keep parsing until we reach the end of the literal/keyword
    const char32_t * parsedStrStartPtr = mLexerState.srcPtr;
    const char32_t * parsedStrEndPtr = parsedStrStartPtr + 1;
    
    while (CharUtils::isValidIdentifierMiddleChar(parsedStrEndPtr[0])) {
        ++parsedStrEndPtr;
    }
    
    size_t parsedStrLen = static_cast<size_t>(parsedStrEndPtr - parsedStrStartPtr);
    
    // Check for these keywords
    // TODO: this is slow, use some sort of binary search method BST
    auto parseKeyword  = [&](const char32_t * keywordStr, TokenType keywordTokenType) -> bool {
        // Compute length of keyword
        size_t keywordLen = StringUtils::strlen(keywordStr);
        
        // If the lengths do not match then it cannot be this keyword
        if (keywordLen != parsedStrLen) {
            return false;
        }
        
        // Go through the keyword string and make sure each char matches with the given string
        const char32_t * parsedStrCurPtr = parsedStrStartPtr;
        const char32_t * keywordCurPtr = keywordStr;
        
        while (true) {
            // Get the current char in both strings
            char32_t keywordChar = keywordCurPtr[0];
            char32_t parsedStrChar = parsedStrCurPtr[0];
            
            // See if we reached the end of the keyword:
            if (keywordChar == 0) {
                break;
            }
            
            // See if we find a mismatch:
            if (keywordChar != parsedStrChar) {
                return false;
            }
            
            // Move onto the next chars
            ++parsedStrCurPtr;
            ++keywordCurPtr;
        }
        
        // Reached keyword end and both strings match: create a keyword token
        allocToken(keywordTokenType, keywordLen);
        
        // Consume the keyword chars
        consumeNumNonWhiteSpaceChars(keywordLen);
        return true;
    };
    
    if (parseKeyword(U"print", TokenType::kPrint) ||
        parseKeyword(U"var", TokenType::kVar))
    {
        return ParseResult::kSuccess;
    }
    
    // If we've got to here we are dealing with an identifier. Allocate enough memory to store the
    // identifier and save:
    Token & tok = allocToken(TokenType::kIdentifier, parsedStrLen);
    tok.data.strVal.ptr = new char32_t[parsedStrLen + 1];
    std::memcpy(tok.data.strVal.ptr, parsedStrStartPtr, parsedStrLen * sizeof(char32_t));
    tok.data.strVal.ptr[parsedStrLen] = 0;
    tok.data.strVal.length = parsedStrLen;
    
    // Consume the identifier chars
    consumeNumNonWhiteSpaceChars(parsedStrLen);
    
    // All good!
    return ParseResult::kSuccess;
}

void Lexer::increaseTokenListCapacity(size_t newCapacity) {
    WC_GUARD(newCapacity >= mTokenCapacity);
    mTokenCapacity = newCapacity;
    mTokenList = reinterpret_cast<Token*>(std::realloc(mTokenList, mTokenCapacity * sizeof(Token)));
}

Token & Lexer::allocToken(TokenType tokenType, size_t tokenSrcLength) {
    // If not enough room, grow capacity according to the Golden ratio (approx 1.6) and add 1 to ensure we alloc at least one token.
    if (mTokenCount + 1 > mTokenCapacity) {
        increaseTokenListCapacity(((mTokenCapacity * 16) / 10) + 1);
    }
    
    // Initialize the token
    Token * token = mTokenList + mTokenCount;
    token->type = tokenType;
    token->srcPtr = mLexerState.srcPtr;
    token->srcLine = mLexerState.srcLine;
    token->srcCol = mLexerState.srcCol;
    token->srcLength = tokenSrcLength;
    token->data = {};
    
#if DEBUG == 1
    // Debug only: put the token into this list for ease of inspection
    mDebugTokenList.push_back(token);
#endif  // #if DEBUG == 1
    
    // Increment token count in use and return
    ++mTokenCount;
    return *token;
}

void Lexer::error(const LexerState & srcLocation, const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Error! Failed to parse the given source into tokens at: line %zu, column %zu\n",
                 srcLocation.srcLine + 1,
                 srcLocation.srcCol + 1);
    
    // Specific error message
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

void Lexer::error(const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Error! Failed to parse the given source into tokens at: line %zu, column %zu\n",
                 mLexerState.srcLine + 1,
                 mLexerState.srcCol + 1);
    
    // Specific error message
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

WC_END_NAMESPACE
