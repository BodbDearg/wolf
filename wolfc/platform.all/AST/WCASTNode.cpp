#include "WCASTNode.hpp"

#include "WCFunc.hpp"
#include "WCModule.hpp"
#include "WCParseCtx.hpp"
#include "WCScope.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdarg>
    #include <cstdio>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

void ASTNode::parseError(ParseCtx & parseCtx,
                         const char * msgFmtStr,
                         ...)
{
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    parseError(parseCtx, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ASTNode::parseError(ParseCtx & parseCtx,
                         const Token & atTok,
                         const char * msgFmtStr,
                         ...)
{
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    parseError(parseCtx, atTok, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void ASTNode::parseError(ParseCtx & parseCtx,
                         const char * msgFmtStr,
                         std::va_list msgFmtStrArgs)
{
    WC_ASSERT(parseCtx.curTok);
    parseError(parseCtx, *parseCtx.curTok, msgFmtStr, msgFmtStrArgs);
}

void ASTNode::parseError(ParseCtx & parseCtx,
                         const Token & atTok,
                         const char * msgFmtStr,
                         std::va_list msgFmtStrArgs)
{
    WC_ASSERT(msgFmtStr);
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    std::sprintf(msgBuf,
                 "Error! Parsing failed at line %zu, column %zu! Message:\n",
                 atTok.startLine + 1,
                 atTok.startCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    parseCtx.errorMsgs.push_back(msgBuf);
    
    // TODO: Remove this and just save the message to the given list
    // Print it to stderr:
    std::fprintf(stderr, "%s", msgBuf);
}

ASTNode::ASTNode() : mParent(nullptr) {
    WC_EMPTY_FUNC_BODY();
}

ASTNode::~ASTNode() {
    WC_EMPTY_FUNC_BODY();
}

Scope * ASTNode::getParentScope() {
    return firstParentOfType<Scope>();
}

const Scope * ASTNode::getParentScope() const {
    return firstParentOfType<Scope>();
}

Func * ASTNode::getParentFunc() {
    return firstParentOfType<Func>();
}

const Func * ASTNode::getParentFunc() const {
    return firstParentOfType<Func>();
}

Module * ASTNode::getParentModule() {
    return firstParentOfType<Module>();
}

const Module * ASTNode::getParentModule() const {
    return firstParentOfType<Module>();
}

void ASTNode::compileError(const char * msg, ...) const {
    // Get the start and end tokens:
    const Token & startToken = getStartToken();
    const Token & endToken = getEndToken();
    
    // Generic error info
    std::fprintf(stderr,
                 "Error! Compile failed for the code beginning at line %zu, column %zu and "
                 "ending at line %zu and column %zu! Message:\n",
                 startToken.startLine + 1,
                 startToken.startCol + 1,
                 endToken.endLine + 1,
                 endToken.endCol + 1);
    
    // Specific error info
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

std::string ASTNode::makeLLVMLabelForTok(const char * labelText, const Token & token) {
    WC_ASSERT(labelText);
    std::string label = labelText;
    label += "@l";
    label += std::to_string(token.startLine + 1);
    label += ", ch";
    label += std::to_string(token.startCol + 1);
    return label;
}

WC_END_NAMESPACE
