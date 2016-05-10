#include "WCLexer.hpp"
#include "WCToken.hpp"
#include "WCCharUtils.hpp"
#include "WCStringUtils.hpp"
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

WCLexer::WCLexer(size_t initialTokenCapacity) :
    mTokenList(nullptr),
    mTokenCapacity(initialTokenCapacity),
    mTokenCount(0),
    mLexerState{}
{
    increaseTokenListCapacity(initialTokenCapacity);
}

WCLexer::~WCLexer() {
    if (mTokenList) {
        std::free(mTokenList);
        mTokenList = nullptr;
    }
}

bool WCLexer::process(const char32_t * srcText) {
    // Init lexer state
    WC_ASSERT(srcText);
    mLexerState = {};
    mLexerState.srcPtr = srcText;
    
    // Continue until there is no source left
    while (char32_t c = mLexerState.srcPtr[0]) {
        // Skip all whitespace before parsing something interesting
        if (consumeWhitespace(c)) {
            continue;
        }
        
        // Try to parse stuff in this order of importance
        if (parseBasicTokens(c)) {
            continue;
        }
        
        if (parseNumericLiteral(c)) {
            continue;
        }
                                               
        // If we get to here then we have an error
        std::unique_ptr<char[]> charAsUtf8(WCStringUtils::convertUtf32ToUtf8(&c, 1));
        error("Unexpected character '%s' at start of token!\n", charAsUtf8.get());
        return false;
    }
    
    // Now add in the end of file token and return true for success
    allocToken(WCTokenType::kEOF);
    return true;
}

bool WCLexer::consumeWhitespace(char32_t currentChar) {
    if (!WCCharUtils::isWhitespace(currentChar)) {
        return false;   // Did not parse whitespace
    }
    
    if (WCCharUtils::isLineSeparator(currentChar)) {
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

void WCLexer::consumeNonWhiteSpace(size_t numChars) {
    mLexerState.srcPtr += numChars;
    mLexerState.srcCol += numChars;
}

bool WCLexer::parseNumericLiteral(char32_t currentChar) {
    // Must start with a digit to be a numeric
    if (!WCCharUtils::isDigit(currentChar)) {
        return false;
    }
    
    // Continue until the end of the numeric literal
    const char32_t * startCharPtr = mLexerState.srcPtr;
    const char32_t * endCharPtr = startCharPtr + 1;
    
    while ((currentChar = endCharPtr[0])) {
        if (WCCharUtils::isDigit(currentChar)) {
            ++endCharPtr;
        }
        else {
            break;
        }
    }
    
    // Skip these in the parser
    size_t numNumericChars = endCharPtr - startCharPtr;
    consumeNonWhiteSpace(numNumericChars);
    
    // Parse the number itself
    const char32_t * currentCharPtr = startCharPtr;
    uint64_t value = 0;
    
    while (currentCharPtr != endCharPtr) {
        // TODO: check for numeric overflow
        uint64_t digit = currentCharPtr[0] - '0';
        value = value * 10 + digit;
        ++currentCharPtr;
    }
    
    // Now make the token
    WCToken & token = allocToken(WCTokenType::kIntLiteral);
    token.mData.intVal = value;
    
    // All good!
    return true;
}

bool WCLexer::parseBasicTokens(char32_t currentChar) {
    switch (currentChar) {
        case '(':
            allocToken(WCTokenType::kLParen);
            consumeNonWhiteSpace(1);
            return true;
            
        case ')':
            allocToken(WCTokenType::kRParen);
            consumeNonWhiteSpace(1);
            return true;
            
        case '+':
            allocToken(WCTokenType::kPlus);
            consumeNonWhiteSpace(1);
            return true;
            
        case '-':
            allocToken(WCTokenType::kMinus);
            consumeNonWhiteSpace(1);
            return true;
            
        case '*':
            allocToken(WCTokenType::kAsterisk);
            consumeNonWhiteSpace(1);
            return true;
            
        case '/':
            allocToken(WCTokenType::kSlash);
            consumeNonWhiteSpace(1);
            return true;
            
        default:
            break;
    }
    
    return false;
}

void WCLexer::increaseTokenListCapacity(size_t newCapacity) {
    if (newCapacity < mTokenCapacity) {
        return;
    }
    
    mTokenCapacity = newCapacity;
    mTokenList = reinterpret_cast<WCToken*>(std::realloc(mTokenList, mTokenCapacity * sizeof(WCToken)));
}

WCToken & WCLexer::allocToken(WCTokenType tokenType) {
    // If not enough room, grow capacity according to the Golden ratio (approx 1.6) and add 1 to ensure we alloc at least one token.
    if (mTokenCount + 1 > mTokenCapacity) {
        increaseTokenListCapacity(((mTokenCapacity * 16) / 10) + 1);
    }
    
    // Initialize the token
    WCToken * token = mTokenList + mTokenCount;
    token->mType = tokenType;
    token->mSrcPtr = mLexerState.srcPtr;
    token->mSrcLine = mLexerState.srcLine;
    token->mSrcCol = mLexerState.srcCol;
    token->mData = {};
    
#if DEBUG == 1
    // Debug only: put the token into this list for ease of inspection
    mDebugTokenList.push_back(token);
#endif  // #if DEBUG == 1
    
    // Increment token count in use and return
    ++mTokenCount;
    return *token;
}

void WCLexer::error(const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Error! Failed to parse the given source into tokens at: line %zu, column %zu\n",
                 (mLexerState.srcLine + 1),
                 (mLexerState.srcCol + 1));
    
    // Specific error message
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
}
