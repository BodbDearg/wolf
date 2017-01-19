#include "CodegenCtx.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenCtx::CodegenCtx() :
    llvmCtx(),
    irBuilder(llvmCtx)
{
    WC_EMPTY_FUNC_BODY();
}

bool CodegenCtx::hasErrors() const {
    return mErrorMsgs.size() > 0;
}

bool CodegenCtx::hasWarnings() const {
    return mWarningMsgs.size() > 0;
}

void CodegenCtx::error(const AST::ASTNode & atNode, const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    error(atNode, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void CodegenCtx::error(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    const Token & startToken = atNode.getStartToken();
    const Token & endToken = atNode.getEndToken();
    
    std::fprintf(stderr,
                 "Error! Compile failed at (line %zu, col %zu)-(line %zu, col %zu)!:\n",
                 startToken.startLine + 1,
                 startToken.startCol + 1,
                 endToken.endLine + 1,
                 endToken.endCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    mErrorMsgs.push_back(msgBuf);
}

void CodegenCtx::warning(const AST::ASTNode & atNode, const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    warning(atNode, msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void CodegenCtx::warning(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    const Token & startToken = atNode.getStartToken();
    const Token & endToken = atNode.getEndToken();
    
    std::fprintf(stderr,
                 "Compile warning at (line %zu, col %zu)-(line %zu, col %zu):\n",
                 startToken.startLine + 1,
                 startToken.startCol + 1,
                 endToken.endLine + 1,
                 endToken.endCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    mWarningMsgs.push_back(msgBuf);
}

void CodegenCtx::pushInsertBlock() {
    mInsertBlockStack.push_back(irBuilder.GetInsertBlock());
    WC_ASSERT(mInsertBlockStack.back() != nullptr);
}

void CodegenCtx::popInsertBlock() {
    WC_ASSERT(!mInsertBlockStack.empty());
    llvm::BasicBlock * basicBlock = mInsertBlockStack.back();
    mInsertBlockStack.pop_back();
    irBuilder.SetInsertPoint(basicBlock);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
