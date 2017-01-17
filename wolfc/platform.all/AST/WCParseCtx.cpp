#include "WCParseCtx.hpp"

#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

ParseCtx::ParseCtx(const Token * startToken, LinearAlloc & linearAlloc) :
    mCurrentToken(startToken),
    mLinearAlloc(linearAlloc)
{
    WC_ASSERT(mCurrentToken);
}

bool ParseCtx::hasErrors() const {
    return !errorMsgs.empty();
}

bool ParseCtx::hasWarnings() const {
    return !warningMsgs.empty();
}

void ParseCtx::error(const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    error(msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ParseCtx::error(const Token & atToken, const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    error(atToken, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ParseCtx::error(const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    error(*mCurrentToken, msgFmtStr, msgFmtStrArgs);
}

void ParseCtx::error(const Token & atToken, const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    std::sprintf(msgBuf,
                 "Error! Parse failed at line %zu, col %zu!: ",
                 atToken.startLine + 1,
                 atToken.startCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    errorMsgs.push_back(msgBuf);
}

void ParseCtx::warning(const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    warning(msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ParseCtx::warning(const Token & atToken, const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    warning(atToken, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ParseCtx::warning(const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    warning(*mCurrentToken, msgFmtStr, msgFmtStrArgs);
}

void ParseCtx::warning(const Token & atToken, const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    std::sprintf(msgBuf,
                 "Parse warning at line %zu, col %zu!: ",
                 atToken.startLine + 1,
                 atToken.startCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    warningMsgs.push_back(msgBuf);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
