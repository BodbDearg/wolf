#include "Lexer.hpp"

#include "Assert.hpp"
#include "CharUtils.hpp"
#include "StringUtils.hpp"
#include "Token.hpp"
#include "DataType/PrimitiveDataTypes.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <memory>
    #include <cstdarg>
    #include <cstdio>
    #include <cstdlib>
WC_THIRD_PARTY_INCLUDES_END

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

bool Lexer::process(const char * utf8Src) {
    // Bootstrap the lexer
    initLexerState(utf8Src);
    
    // Continue until there is no source left
    while (!mLexerState.error && mLexerState.currentChar != 0) {
        // Skip all whitespace before parsing something interesting
        if (trySkipWhitespaceChar()) {
            continue;
        }
        
        // If there was a lexer error then abort:
        WC_GUARD(!mLexerState.error, false);
        
        // Try to parse stuff in this order of importance
        #define INVOKE_PARSE_FUNC(func)\
            \
            {\
                ParseResult result = func();\
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
        INVOKE_PARSE_FUNC(parseSingleLineComments);
        INVOKE_PARSE_FUNC(parseMultiLineComments);
                                               
        // If we get to here then we have an error
        std::string charAsUtf8Str(mLexerState.srcPtr, mLexerState.currentCharNumBytes);
        error("Unexpected character '%s' at start of token!", charAsUtf8Str.c_str());
        return false;
    }
    
    // If there was an error return the code for that
    if (mLexerState.error) {
        return false;
    }
    
    // Now add in the end of file token and return true for success
    createEOFToken();
    return true;
}

const Token * Lexer::getTokenList() const {
    return mTokenList;
}

bool Lexer::initLexerState(const char * utf8Src) {
    WC_ASSERT(utf8Src);
    mLexerState.error = false;
    mLexerState.srcPtr = utf8Src;
    mLexerState.srcLine = 0;
    mLexerState.srcCol = 0;
    
    if (!StringUtils::parseUtf8Char(utf8Src, mLexerState.currentChar, mLexerState.currentCharNumBytes)) {
        error("Malformed utf8 bytes at the start of the source code! Unable to parse!");
        mLexerState.currentCharIsNewline = false;
        return false;
    }
    
    mLexerState.currentCharIsNewline = CharUtils::isLineSeparator(mLexerState.currentChar);
    return true;
}

bool Lexer::trySkipWhitespaceChar() {
    // Must be a whitespace char to consume at least one!
    char32_t currentChar = mLexerState.currentChar;
    WC_GUARD(CharUtils::isWhitespace(currentChar), false);
    
    // Skip this character:
    moveOntoNextChar();
    
    // Parsed a whitespace:
    return true;
}

void Lexer::skipAnyWhitespaceCharsAhead() {
    while (!mLexerState.error && mLexerState.currentChar != 0) {
        if (!trySkipWhitespaceChar()) {
            break;
        }
    }
}

bool Lexer::moveOntoNextChar() {
    // Move along by one char, see if the next char is a newline or just a regular non newline char
    if (mLexerState.currentCharIsNewline) {
        // Save a newline token into the list of tokens
        Token & newlineTok = allocToken(TokenType::kNewline);
        newlineTok.startLine = mLexerState.srcLine;
        newlineTok.startCol = mLexerState.srcCol;
        newlineTok.startSrcPtr = mLexerState.srcPtr;
        newlineTok.endLine = mLexerState.srcLine + 1;
        newlineTok.endCol = 0;
        newlineTok.endSrcPtr = mLexerState.srcPtr + 1;
        
        // Newline char: have to treat this different
        if (mLexerState.currentChar == '\r' && mLexerState.srcPtr[1] == '\n') {
            // Carriage return and newline utf8 combo: interpret as just one newline
            mLexerState.srcPtr += 2;
            mLexerState.srcLine++;
            mLexerState.srcCol = 0;
            
            // Move on the end for the newline token by one character
            newlineTok.endSrcPtr++;
        }
        else {
            // Regular new line
            mLexerState.srcPtr += mLexerState.currentCharNumBytes;
            mLexerState.srcLine++;
            mLexerState.srcCol = 0;
        }
    }
    else {
        // Regular char, treat as normal
        mLexerState.srcPtr += mLexerState.currentCharNumBytes;
        mLexerState.srcCol++;
    }
    
    // Alright, now see what the new character is
    if (!StringUtils::parseUtf8Char(mLexerState.srcPtr,
                                    mLexerState.currentChar,
                                    mLexerState.currentCharNumBytes))
    {
        mLexerState.error = true;
        error("Malformed utf8 bytes at the current location in the source code! Unable to parse!");
        return false;
    }
    
    // See if it was a newline
    mLexerState.currentCharIsNewline = CharUtils::isLineSeparator(mLexerState.currentChar);
    return true;
}

Lexer::ParseResult Lexer::parseBasicTokens() {
    switch (mLexerState.currentChar) {
        case '(': return parseBasicToken(TokenType::kLParen, 1);
        case ')': return parseBasicToken(TokenType::kRParen, 1);
        case '[': return parseBasicToken(TokenType::kLBrack, 1);
        case ']': return parseBasicToken(TokenType::kRBrack, 1);
            
        case '+': {
            if (mLexerState.srcPtr[1] == '+') {
                // '++' operator
                return parseBasicToken(TokenType::kIncrement, 2);
            }
            else if (mLexerState.srcPtr[1] == '=') {
                // '+=' operator
                return parseBasicToken(TokenType::kAssignAdd, 2);
            }
            else {
                // '+' operator
                return parseBasicToken(TokenType::kPlus, 1);
            }
        }
        
        case '-': {
            if (mLexerState.srcPtr[1] == '>') {
                // '->' operator
                return parseBasicToken(TokenType::kOpArrow, 2);
            }
            else if (mLexerState.srcPtr[1] == '-') {
                // '--' operator
                return parseBasicToken(TokenType::kDecrement, 2);
            }
            else if (mLexerState.srcPtr[1] == '=') {
                // '-=' operator
                return parseBasicToken(TokenType::kAssignSub, 2);
            }
            else {
                // '-' operator
                return parseBasicToken(TokenType::kMinus, 1);
            }
        }
        
        case '%': {
            if (mLexerState.srcPtr[1] == '=') {
                // '%=' operator
                return parseBasicToken(TokenType::kAssignMod, 2);
            }
            else {
                // '%' operator
                return parseBasicToken(TokenType::kPercent, 1);
            }
        }
        
        case '*': {
            if (mLexerState.srcPtr[1] == '=') {
                // '*=' operator
                return parseBasicToken(TokenType::kAssignMul, 2);
            }
            else {
                // '*' operator
                return parseBasicToken(TokenType::kAsterisk, 1);
            }
        }
        
        case '/': {
            // '/' can be followed immediately by ';', in which case it is a multi line comment opener
            if (mLexerState.srcPtr[1] == ';') {
                // Multi line comment: do not interpret the '/' as a token
                return ParseResult::kNone;
            }
            else if (mLexerState.srcPtr[1] == '=') {
                // '/=' operator
                return parseBasicToken(TokenType::kAssignDiv, 2);
            }
            else {
                // '/' operator
                return parseBasicToken(TokenType::kSlash, 1);
            }
        }
            
        case '<': {
            if (mLexerState.srcPtr[1] == '=') {
                // '<=' operator
                return parseBasicToken(TokenType::kCmpLE, 2);
            }
            else if (mLexerState.srcPtr[1] == '<') {
                if (mLexerState.srcPtr[2] == '=') {
                    // '<<=' operator
                    return parseBasicToken(TokenType::kAssignLShift, 3);
                }
                else {
                    // '<<' operator
                    return parseBasicToken(TokenType::kLShift, 2);
                }
            }
            else {
                // '<' operator
                return parseBasicToken(TokenType::kCmpLT, 1);
            }
        }
            
        case '>': {
            if (mLexerState.srcPtr[1] == '=') {
                // '>=' operator
                return parseBasicToken(TokenType::kCmpGE, 2);
            }
            else if (mLexerState.srcPtr[1] == '>') {
                if (mLexerState.srcPtr[2] == '>') {
                    if (mLexerState.srcPtr[3] == '=') {
                        // '>>>=' operator
                        return parseBasicToken(TokenType::kAssignLRShift, 4);
                    }
                    else {
                        // '>>>' operator
                        return parseBasicToken(TokenType::kLRShift, 3);
                    }
                }
                else {
                    if (mLexerState.srcPtr[2] == '=') {
                        // '>>=' operator
                        return parseBasicToken(TokenType::kAssignARShift, 3);
                    }
                    else {
                        // '>>=' operator
                        return parseBasicToken(TokenType::kARShift, 2);
                    }
                }
            }
            else {
                // '>' operator
                return parseBasicToken(TokenType::kCmpGT, 1);
            }
        }
            
        case '=': {
            if (mLexerState.srcPtr[1] == '=') {
                // '==' operator
                return parseBasicToken(TokenType::kCmpEQ, 2);
            }
            else {
                // '=' operator
                return parseBasicToken(TokenType::kAssign, 1);
            }
        }
            
        case '!': {
            if (mLexerState.srcPtr[1] == '=') {
                // '!=' operator
                return parseBasicToken(TokenType::kCmpNE, 2);
            }
            else {
                // '!' operator
                return parseBasicToken(TokenType::kExclamation, 1);
            }
        }
            
        case ',': return parseBasicToken(TokenType::kComma, 1);
        case ':': return parseBasicToken(TokenType::kColon, 1);
        case '?': return parseBasicToken(TokenType::kQMark, 1);
        case '~': return parseBasicToken(TokenType::kTilde, 1);
            
        case '&': {
            if (mLexerState.srcPtr[1] == '=') {
                // '&=' operator
                return parseBasicToken(TokenType::kAssignBAnd, 2);
            }
            else {
                // '&' operator
                return parseBasicToken(TokenType::kAmpersand, 1);
            }
        }
            
        case '^': {
            if (mLexerState.srcPtr[1] == '=') {
                // '^=' operator
                return parseBasicToken(TokenType::kAssignBXor, 2);
            }
            else {
                // '^' operator
                return parseBasicToken(TokenType::kHat, 1);
            }
        }
        
        case '|': {
            if (mLexerState.srcPtr[1] == '=') {
                // '|=' operator
                return parseBasicToken(TokenType::kAssignBOr, 2);
            }
            else {
                // '|' operator
                return parseBasicToken(TokenType::kVBar, 1);
            }
        }
            
        default:
            break;
    }
    
    return ParseResult::kNone;
}

Lexer::ParseResult Lexer::parseBasicToken(TokenType tokenType, size_t numCharsInToken) {
    Token & tok = allocToken(tokenType);
    tok.startLine = mLexerState.srcLine;
    tok.startCol = mLexerState.srcCol;
    tok.startSrcPtr = mLexerState.srcPtr;
    
    while (numCharsInToken > 0) {
        --numCharsInToken;
        
        if (!moveOntoNextChar()) {
            return ParseResult::kFail;
        }
    }
    
    tok.endLine = mLexerState.srcLine;
    tok.endCol = mLexerState.srcCol;
    tok.endSrcPtr = mLexerState.srcPtr;
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseNumericLiteral() {
    // Must start with a digit to be a numeric
    const char32_t firstChar = mLexerState.currentChar;
    WC_GUARD(CharUtils::isDecimalDigit(firstChar), ParseResult::kNone);
    
    // Save the start of the literal and move onto the next char
    const char * tokStartPtr = mLexerState.srcPtr;
    const size_t tokStartLine = mLexerState.srcLine;
    const size_t tokStartCol = mLexerState.srcCol;
    WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
    
    // See what type of literal we are dealing with, base 2, 8, 10, or 16.
    // Once we figure out that parse until the end of the iteral:
    const char32_t secondChar = mLexerState.currentChar;
    size_t numberBase = 10;
    
    if (firstChar == '0') {
        if (secondChar == 'b' || secondChar == 'B') {
            // 0b or 0B: Binary integer literal:
            if (!CharUtils::isBinaryDigit(mLexerState.currentChar)) {
                error("Expect a binary digit (0 or 1) at start of binary integer literal!");
                return ParseResult::kFail;
            }
            
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            numberBase = 2;
            
            while (CharUtils::isBinaryDigitOrUnderscore(mLexerState.currentChar)) {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            }
        }
        else if (secondChar == 'x' || secondChar == 'X') {
            // 0x or 0X: Hex integer literal:
            if (!CharUtils::isHexDigit(mLexerState.currentChar)) {
                error("Expect a hex digit (0-9 or a-f/A-F) at start of hex integer literal!");
                return ParseResult::kFail;
            }
            
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            numberBase = 16;
            
            while (CharUtils::isHexDigitOrUnderscore(mLexerState.currentChar)) {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            }
        }
        else if (secondChar == 'o' || secondChar == 'O') {
            // 0o or 0O: Octal integer literal:
            if (!CharUtils::isOctalDigit(mLexerState.currentChar)) {
                error("Expect an octal digit (0-7) at start of octal integer literal!");
                return ParseResult::kFail;
            }
            
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            numberBase = 8;
            
            while (CharUtils::isOctalDigitOrUnderscore(mLexerState.currentChar)) {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            }
        }
        else {
            // Regular decimal integer literal
            while (CharUtils::isDecimalDigitOrUnderscore(mLexerState.currentChar)) {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            }
        }
    }
    else {
        // Regular decimal integer literal
        while (CharUtils::isDecimalDigitOrUnderscore(mLexerState.currentChar)) {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
        }
    }
    
    // Save the end of the integer literal digits
    const char * intDigitsEndPtr = mLexerState.srcPtr;
    
    // Okay, parse the integer data type if there:
    DataTypeId dataTypeId = PrimitiveDataTypes::getDefaultIntTypeId();
    
    if (mLexerState.currentChar == 'i' || mLexerState.currentChar == 'I') {
        WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
        
        if (mLexerState.currentChar == '8') {
            // i8:
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            dataTypeId = DataTypeId::kInt8;
        }
        else if (mLexerState.currentChar == '1') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);

            if (mLexerState.currentChar == '2') {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                
                if (mLexerState.currentChar == '8') {
                    // i128:
                    WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                    dataTypeId = DataTypeId::kInt128;
                }
            }
            else if (mLexerState.currentChar == '6') {
                // i16:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kInt16;
            }
        }
        else if (mLexerState.currentChar == '3') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            
            if (mLexerState.currentChar == '2') {
                // i32:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kInt32;
            }
        }
        else if (mLexerState.currentChar == '6') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            
            if (mLexerState.currentChar == '4') {
                // i64:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kInt64;
            }
        }
    }
    else if (mLexerState.srcPtr[0] == 'u' || mLexerState.srcPtr[0] == 'U') {
        WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
        
        if (mLexerState.currentChar == '8') {
            // u8:
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            dataTypeId = DataTypeId::kUInt8;
        }
        else if (mLexerState.currentChar == '1') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);

            if (mLexerState.currentChar == '2') {
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                
                if (mLexerState.currentChar == '8') {
                    // u128:
                    WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                    dataTypeId = DataTypeId::kUInt128;
                }
            }
            else if (mLexerState.currentChar == '6') {
                // u16:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kUInt16;
            }
        }
        else if (mLexerState.currentChar == '3') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            
            if (mLexerState.currentChar == '2') {
                // u32:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kUInt32;
            }
        }
        else if (mLexerState.currentChar == '6') {
            WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
            
            if (mLexerState.currentChar == '4') {
                // u64:
                WC_GUARD(moveOntoNextChar(), ParseResult::kFail);
                dataTypeId = DataTypeId::kUInt64;
            }
        }
    }
    
    // Literal should end on a non alpha numeric character:
    if (CharUtils::isAlphaNumeric(mLexerState.currentChar)) {
        error("Expect a non alpha numeric character (like space) or a valid integer precision suffix like 'i' "
              "or 'i32' etc. to follow the end of an integer literal!");
        
        return ParseResult::kFail;
    }
    
    // Now make the token and finish up
    Token & token = allocToken(TokenType::kIntLit);
    token.startSrcPtr = tokStartPtr;
    token.startLine = tokStartLine;
    token.startCol = tokStartCol;
    token.endSrcPtr = mLexerState.srcPtr;
    token.endLine = mLexerState.srcLine;
    token.endCol = mLexerState.srcCol;
    
    auto & tokIntData = token.data.intData;
    tokIntData.base = numberBase;
    tokIntData.dataTypeId = dataTypeId;
    
    // Makeup the string for the integer literal without underscores:
    // TODO: What allocates this string?
    char * intLitStrWithoutUS = new char[
        static_cast<size_t>(intDigitsEndPtr - token.startSrcPtr) + 1
    ];
    
    tokIntData.strMinusUS = intLitStrWithoutUS;
    size_t & tokLenMinusUS = tokIntData.strMinusUSLen;
    tokLenMinusUS = 0;
    
    {
        // Run through the string and make it up without underscores
        const char * src = token.startSrcPtr;
        char * dst = intLitStrWithoutUS;
        
        while (src < intDigitsEndPtr) {
            char c = src[0];
            
            if (c == 0) {
                break;
            }
            
            if (c != '_') {
                *dst = c;
                ++dst;
                ++tokLenMinusUS;
            }
            
            ++src;
        }
        
        // Null termiante the string
        *dst = 0;
    }
    
    // All went well
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseDoubleQuotedStringLiteral() {
    // Must start with a double quote
    WC_GUARD(mLexerState.currentChar == '"', ParseResult::kNone);
    
    // Alright skip the opening '"'
    const char * tokStartPtr = mLexerState.srcPtr;
    size_t tokStartLine = mLexerState.srcLine;
    size_t tokStartCol = mLexerState.srcCol;
    
    if (!moveOntoNextChar()) {
        return ParseResult::kFail;
    }
    
    // Save the start of the actual string data
    const char * strStartPtr = mLexerState.srcPtr;
    
    // Search for the end of the string until we reach '"':
    while (mLexerState.currentChar != '"') {
        // Bail with an error if we hit null
        if (mLexerState.currentChar == 0) {
            error("Unexpected EOF while parsing double quoted string! Is the closing '\"' present?");
            return ParseResult::kFail;
        }
        
        // If we hit a newline then that is an error also
        if (CharUtils::isLineSeparator(mLexerState.currentChar)) {
            error("Unexpected newline while parsing double quoted string! Newline characters must be escaped!");
            return ParseResult::kFail;
        }
        
        // Consume the char and move onto the next
        if (!moveOntoNextChar()) {
            return ParseResult::kFail;
        }
    }
    
    // Now we have the end of the string data
    const char * strEndPtr = mLexerState.srcPtr;
    
    // Skip the end '"'
    if (!moveOntoNextChar()) {
        return ParseResult::kFail;
    }
    
    const char * tokEndPtr = mLexerState.srcPtr;
    size_t tokEndLine = mLexerState.srcLine;
    size_t tokEndCol = mLexerState.srcCol;
    
    // Compute the size of the buffer we need to hold the string in:
    size_t strBufferSize = static_cast<size_t>(strEndPtr - strStartPtr + 1);
    
    // Allocate the token for this literal
    Token & tok = allocToken(TokenType::kStrLit);
    tok.startSrcPtr = tokStartPtr;
    tok.startLine = tokStartLine;
    tok.startCol = tokStartCol;
    tok.endSrcPtr = tokEndPtr;
    tok.endLine = tokEndLine;
    tok.endCol = tokEndCol;
    
    // Now decode the actual string data into the literal.
    // FIXME: what manages this memory?
    tok.data.strVal.ptr = new char[strBufferSize];
    char * decodedStrPtr = tok.data.strVal.ptr;
    
    // Now decode the string into the buffer
    for (const char * strCurPtr = strStartPtr; strCurPtr != strEndPtr; ++strCurPtr) {
        // See if escaped char:
        char c1 = strCurPtr[0];
        
        if (c1 == '\\') {
            // Possible escaped char: must not be at the end of the string though
            if (strCurPtr + 1 >= strEndPtr) {
                error(tokStartLine,
                      tokStartCol + 1 + static_cast<size_t>(strCurPtr - strStartPtr),
                      "Unexpected end string while parsing '\\' escape sequence!");
                
                return ParseResult::kFail;
            }
            
            // Read ahead and see what char we have
            char c2 = strCurPtr[1];
            
            // Now parse the escaped char
            char escapedChar = 0;
            
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
                char32_t escapedCharUtf32 = 0;
                size_t escapedCharNumUtf8Bytes = 0;

                if (!StringUtils::parseUtf8Char(strCurPtr, escapedCharUtf32, escapedCharNumUtf8Bytes)) {
                    WC_RAISE_ASSERTION("Should't ever reach this case! Must be valid utf8 if we reached this point.");
                    return ParseResult::kFail;
                }
                
                std::string escapedCharUtf8(strCurPtr, escapedCharNumUtf8Bytes);
                error(tokStartLine,
                      tokStartCol + 1 + static_cast<size_t>(strCurPtr - strStartPtr),
                      "Unrecognized escape sequence '\\%s'!",
                      escapedCharUtf8.c_str());
                
                return ParseResult::kFail;
            }
            
            // Save the escaped char. Note: Need to skip two characters for this
            decodedStrPtr[0] = escapedChar;
            ++strCurPtr;
        }
        else {
            // Ordinary char, parse as normal
            decodedStrPtr[0] = c1;
        }
        
        // Move along in the decoded string buffer
        ++decodedStrPtr;
        
        // Increase the size of the parsed string
        tok.data.strVal.size++;
    }
    
    // Null terminate the decoded string when done
    tok.data.strVal.ptr[tok.data.strVal.size] = 0;
    
    // Save the index of the current token we are on, will need down below
    size_t thisStrLitTokenNum = mTokenCount;

    // Try to parse another string: if we find multiple string literals in succession
    // then join them together (similar to C/C++)
    skipAnyWhitespaceCharsAhead();
    ParseResult nextStringParseResult = parseDoubleQuotedStringLiteral();

    if (nextStringParseResult != ParseResult::kNone) {
        // Another string literal follows, see if that was parsed ok:
        if (nextStringParseResult == ParseResult::kSuccess) {
            // Get the length of the next string literal:
            Token & nextTok = mTokenList[mTokenCount - 1];
            auto & nextTokStrVal = nextTok.data.strVal;
            size_t nextStrLen = nextTokStrVal.size;

            // If the next string is zero length then just pop the token for it and return this string:
            if (nextStrLen == 0) {
                // Zero length string ahead, just pop it off (and any newlines in between) to consume it
                while (mTokenCount > thisStrLitTokenNum) {
                    popToken();
                }
            }
            else {
                // Alright, we want to concatenate the strings from the two sources:
                auto & curTokStrVal = tok.data.strVal;
                size_t curStrLen = tok.data.strVal.size;
                size_t concatStrSize = curStrLen + nextStrLen + 1;

                // FIXME: what manages this memory?
                // Concatenate the two strings:
                char * concatStrPtr = new char[concatStrSize];
                
                {
                    char * curConcatStrPtr = concatStrPtr;
                    std::memcpy(curConcatStrPtr, curTokStrVal.ptr, curStrLen);
                    curConcatStrPtr += curStrLen;
                    std::memcpy(curConcatStrPtr, nextTokStrVal.ptr, nextStrLen);
                    curConcatStrPtr += nextStrLen;
                    *curConcatStrPtr = 0;
                }

                // FIXME: how to free the old memory?
                // Now save the newly concatenated string details on the CURRENT string
                curTokStrVal.ptr = concatStrPtr;
                curTokStrVal.size = concatStrSize;

                // Done with the next token, pop it off the stack (and any newlines in between)
                while (mTokenCount > thisStrLitTokenNum) {
                    popToken();
                }
            }
        }
        else {
            // Failed to parse the following string literal. Therefore parsing of this string fails too:
            return ParseResult::kFail;
        }
    }

    // Succeeded in parsing this string!
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseKeywordsAndLiterals() {
    // Check start character is ok
    WC_GUARD(CharUtils::isValidIdentifierStartChar(mLexerState.currentChar), Lexer::ParseResult::kNone);
    
    // Keep parsing until we reach the end of the literal/keyword
    const char * tokStartPtr = mLexerState.srcPtr;
    size_t tokStartLine = mLexerState.srcLine;
    size_t tokStartCol = mLexerState.srcCol;
    
    if (!moveOntoNextChar()) {
        return ParseResult::kFail;
    }
    
    while (CharUtils::isValidIdentifierMiddleChar(mLexerState.currentChar)) {
        if (!moveOntoNextChar()) {
            return ParseResult::kFail;
        }
    }
    
    const char * tokEndPtr = mLexerState.srcPtr;
    size_t tokEndLine = mLexerState.srcLine;
    size_t tokEndCol = mLexerState.srcCol;
    size_t tokLenInCodeUnits = static_cast<size_t>(tokEndPtr - tokStartPtr);
    
    // Check for these keywords
    // TODO: this is slow, use some sort of binary search method BST
    auto parseKeyword = [&](const char * keywordStr, TokenType keywordTokenType) -> bool {
        // Compute length of keyword
        size_t keywordLenInCodeUnits = std::strlen(keywordStr);
        
        // If the lengths do not match then it cannot be this keyword
        if (keywordLenInCodeUnits != tokLenInCodeUnits) {
            return false;
        }
        
        // Go through the keyword string and make sure each char matches with the given string
        const char * tokCurPtr = tokStartPtr;
        const char * keywordCurPtr = keywordStr;
        
        while (true) {
            // Get the current char in both strings
            char keywordChar = keywordCurPtr[0];
            char tokChar = tokCurPtr[0];
            
            // See if we reached the end of the keyword:
            if (keywordChar == 0) {
                break;
            }
            
            // See if we find a mismatch:
            if (keywordChar != tokChar) {
                return false;
            }
            
            // Move onto the next chars
            ++tokCurPtr;
            ++keywordCurPtr;
        }
        
        // Reached keyword end and both strings match: create a keyword token
        Token & tok = allocToken(keywordTokenType);
        tok.startSrcPtr = tokStartPtr;
        tok.startLine = tokStartLine;
        tok.startCol = tokStartCol;
        tok.endSrcPtr = tokEndPtr;
        tok.endLine = tokEndLine;
        tok.endCol = tokEndCol;
        return true;
    };
    
    if (parseKeyword("true", TokenType::kTrue) ||
        parseKeyword("false", TokenType::kFalse) ||
        parseKeyword("void", TokenType::kVoid) ||
        parseKeyword("int", TokenType::kInt) ||
        parseKeyword("int8", TokenType::kInt8) ||
        parseKeyword("int16", TokenType::kInt16) ||
        parseKeyword("int32", TokenType::kInt32) ||
        parseKeyword("int64", TokenType::kInt64) ||
        parseKeyword("int128", TokenType::kInt128) ||
        parseKeyword("uint", TokenType::kUInt) ||
        parseKeyword("uint8", TokenType::kUInt8) ||
        parseKeyword("uint16", TokenType::kUInt16) ||
        parseKeyword("uint32", TokenType::kUInt32) ||
        parseKeyword("uint64", TokenType::kUInt64) ||
        parseKeyword("uint128", TokenType::kUInt128) ||
        parseKeyword("bool", TokenType::kBool) ||
        parseKeyword("string", TokenType::kString) ||
        parseKeyword("let", TokenType::kLet) ||
        parseKeyword("is", TokenType::kIs) ||
        parseKeyword("and", TokenType::kAnd) ||
        parseKeyword("or", TokenType::kOr) ||
        parseKeyword("not", TokenType::kNot) ||
        parseKeyword("if", TokenType::kIf) ||
        parseKeyword("unless", TokenType::kUnless) ||
        parseKeyword("then", TokenType::kThen) ||
        parseKeyword("else", TokenType::kElse) ||
        parseKeyword("end", TokenType::kEnd) ||
        parseKeyword("while", TokenType::kWhile) ||
        parseKeyword("until", TokenType::kUntil) ||
        parseKeyword("do", TokenType::kDo) ||
        parseKeyword("break", TokenType::kBreak) ||
        parseKeyword("next", TokenType::kNext) ||
        parseKeyword("loop", TokenType::kLoop) ||
        parseKeyword("repeat", TokenType::kRepeat) ||
        parseKeyword("scope", TokenType::kScope) ||
        parseKeyword("noop", TokenType::kNoOp) ||
        parseKeyword("return", TokenType::kReturn) ||
        parseKeyword("func", TokenType::kFunc) ||
        parseKeyword("cast", TokenType::kCast) ||
        parseKeyword("to", TokenType::kTo) ||
        parseKeyword("time", TokenType::kTime) ||
        parseKeyword("rand", TokenType::kRand) ||
        parseKeyword("srand", TokenType::kSRand) ||
        parseKeyword("print", TokenType::kPrint) ||
        parseKeyword("readnum", TokenType::kReadnum) ||
        parseKeyword("assert", TokenType::kAssert))
    {
        return ParseResult::kSuccess;
    }
    
    // TODO: Figure out memory management here
    // If we've got to here we are dealing with an identifier.
    // Allocate enough memory to store the identifier and save:
    Token & tok = allocToken(TokenType::kIdentifier);
    tok.startSrcPtr = tokStartPtr;
    tok.startLine = tokStartLine;
    tok.startCol = tokStartCol;
    tok.endSrcPtr = tokEndPtr;
    tok.endLine = tokEndLine;
    tok.endCol = tokEndCol;
    
    tok.data.strVal.ptr = new char[tokLenInCodeUnits + 1];
    std::memcpy(tok.data.strVal.ptr, tokStartPtr, tokLenInCodeUnits * sizeof(char));
    tok.data.strVal.ptr[tokLenInCodeUnits] = 0;
    tok.data.strVal.size = tokLenInCodeUnits;
    
    // All good!
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseSingleLineComments() {
    // Check start character is the start of a single or multi line comment: ';'
    WC_GUARD(mLexerState.currentChar == ';', ParseResult::kNone);
    
    // Keep skipping chars until a newline is encountered
    while (moveOntoNextChar() && mLexerState.currentChar != 0) {
        if (CharUtils::isLineSeparator(mLexerState.currentChar)) {
            break;  // Newline reached: end of comment
        }
    }
    
    // Single line comments always succeed!
    return ParseResult::kSuccess;
}

Lexer::ParseResult Lexer::parseMultiLineComments() {
    // Multi line comments must start with '/;'
    WC_GUARD(mLexerState.currentChar == '/', ParseResult::kNone);
    
    // Peek the next char after that, must be ';'
    WC_GUARD(mLexerState.srcPtr[1] == ';', ParseResult::kNone);
    
    // Skip the first two chars
    moveOntoNextChar();
    
    while (moveOntoNextChar() && mLexerState.currentChar != 0) {
        // Look for the end two chars:
        if (mLexerState.currentChar == ';' && mLexerState.srcPtr[1] == '/') {
            // End of the multi line comment block, consume the closing '/'
            moveOntoNextChar();
            moveOntoNextChar();
            
            // Parsed successfully!
            return ParseResult::kSuccess;
        }
    }
    
    // If we reached the end of file before the comment is terminated then that is an error
    error("Unexpected EOF reached while parsing multi line comment! Did you forget the closing ';/' ?");
    return ParseResult::kFail;
}

void Lexer::increaseTokenListCapacity(size_t newCapacity) {
    WC_GUARD(newCapacity >= mTokenCapacity);
    mTokenCapacity = newCapacity;
    mTokenList = reinterpret_cast<Token*>(std::realloc(mTokenList, mTokenCapacity * sizeof(Token)));
}

Token & Lexer::allocToken(TokenType tokenType)
{
    // If not enough room, grow capacity according to the Golden ratio (approx 1.6) and add 1 to ensure we alloc at least one token.
    if (mTokenCount + 1 > mTokenCapacity) {
        increaseTokenListCapacity(((mTokenCapacity * 16) / 10) + 1);
    }
    
    // Initialize the token
    Token * token = mTokenList + mTokenCount;
    std::memset(token, 0, sizeof(Token));
    token->type = tokenType;
    
#if DEBUG == 1
    // Debug only: put the token into this list for ease of inspection
    mDebugTokenList.push_back(token);
#endif  // #if DEBUG == 1
    
    // Increment token count in use and return
    ++mTokenCount;
    return *token;
}

void Lexer::popToken() {
    WC_ASSERT(mTokenCount > 0);

#if DEBUG == 1
    // Debug only: pop the token off this debug list
    mDebugTokenList.pop_back();
#endif  // #if DEBUG == 1
    
    // TODO: cleanup token data here?
    --mTokenCount;
}

void Lexer::createEOFToken() {
    Token & tok = allocToken(TokenType::kEOF);
    tok.startLine = mLexerState.srcLine;
    tok.endLine = mLexerState.srcLine;
    tok.startCol = mLexerState.srcCol;
    tok.endCol = mLexerState.srcCol;
    tok.startSrcPtr = mLexerState.srcPtr;
    tok.endSrcPtr = mLexerState.srcPtr;
}

void Lexer::error(size_t srcLine, size_t srcCol, const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Error! Failed to parse the given source into tokens at: line %zu, column %zu\n",
                 srcLine + 1,
                 srcCol + 1);
    
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
