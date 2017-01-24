#include "CodegenCtx.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCFunc.hpp"
#include "DataType/Primitives/WCUnknownDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Function.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenCtx::CodegenCtx() :
    mLLVMCtx(),
    mIRBuilder(mLLVMCtx)
{
    WC_EMPTY_FUNC_BODY();
}

CodegenCtx::~CodegenCtx() {
    // Just here so we can forward declare some types in the header
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
    
    std::sprintf(msgBuf,
                 "@line %zu, col %zu: Error! ",
                 startToken.startLine + 1,
                 startToken.startCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    mErrorMsgs.push_back(msgBuf);
}

void CodegenCtx::error(const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    error(msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void CodegenCtx::error(const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // If we have a node for line info at then use the standard error message function
    if (!mASTNodeStack.empty()) {
        error(*mASTNodeStack.back(), msgFmtStr, msgFmtStrArgs);
        return;
    }
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    std::sprintf(msgBuf, "@Unknown location: Error! ");
    
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
    
    std::sprintf(msgBuf,
                 "@line %zu, col %zu: Warning: ",
                 startToken.startLine + 1,
                 startToken.startCol + 1);
    
    // Get how long the message is so far:
    size_t msgPrefixLen = std::strlen(msgBuf);
    
    // Print the message to the buffer
    vsnprintf(msgBuf + msgPrefixLen, kMaxMsgLen - msgPrefixLen, msgFmtStr, msgFmtStrArgs);
    
    // Ensure it is terminated
    msgBuf[kMaxMsgLen - 1] = 0;
    
    // Save the message:
    mWarningMsgs.push_back(msgBuf);
}

void CodegenCtx::warning(const char * msgFmtStr, ...) {
    va_list msgFmtStrArgs;
    va_start(msgFmtStrArgs, msgFmtStr);
    warning(msgFmtStr, msgFmtStrArgs);
    va_end(msgFmtStrArgs);
}

void CodegenCtx::warning(const char * msgFmtStr, std::va_list msgFmtStrArgs) {
    WC_ASSERT(msgFmtStr);
    
    // If we have a node for line info at then use the standard warning message function
    if (!mASTNodeStack.empty()) {
        warning(*mASTNodeStack.back(), msgFmtStr, msgFmtStrArgs);
        return;
    }
    
    // TODO: support really really long strings here, larger than 16K
    // For super long strings we should try to allocate on the heap
    const size_t kMaxMsgLen = 1024 * 16;
    char msgBuf[kMaxMsgLen];
    
    // Print the start of the message
    std::sprintf(msgBuf, "@Unknown location: Warning: ");
    
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
    mInsertBlockStack.push_back(mIRBuilder.GetInsertBlock());
    WC_ASSERT(mInsertBlockStack.back() != nullptr);
}

void CodegenCtx::popInsertBlock() {
    WC_ASSERT(!mInsertBlockStack.empty());
    llvm::BasicBlock * basicBlock = mInsertBlockStack.back();
    mInsertBlockStack.pop_back();
    mIRBuilder.SetInsertPoint(basicBlock);
}

bool CodegenCtx::dumpIRCodeToStdout() {
    WC_GUARD(mLLVMModule.get(), false);
    mLLVMModule->dump();
    return true;
}

bool CodegenCtx::registerModuleFunc(const AST::Func & astNode) {
    // The function must not already be registered
    if (getModuleFunc(astNode.name()) != nullptr) {
        error(astNode, "A function named '%s' is already defined! Cannot be redefined!", astNode.name());
        return false;
    }
    
    mFuncs[astNode.name()].reset(new Function(astNode));
    return true;
}

Function * CodegenCtx::getModuleFunc(const char * name) {
    auto iter = mFuncs.find(name);
    WC_GUARD(iter != mFuncs.end(), nullptr);
    return iter->second.get();
}

Function * CodegenCtx::getModuleFunc(const std::string & name) {
    auto iter = mFuncs.find(name);
    WC_GUARD(iter != mFuncs.end(), nullptr);
    return iter->second.get();
}

void CodegenCtx::pushASTNode(const AST::ASTNode & node) {
    mASTNodeStack.push_back(&node);
}

void CodegenCtx::popASTNode() {
    WC_ASSERT(!mASTNodeStack.empty());
    mASTNodeStack.pop_back();
}

void CodegenCtx::pushLLVMValue(llvm::Value & llvmValue) {
    mLLVMValues.push_back(&llvmValue);
}

llvm::Value * CodegenCtx::popLLVMValue() {
    if (mLLVMValues.empty()) {
        return nullptr;
    }
    
    llvm::Value * llvmValue = mLLVMValues.back();
    mLLVMValues.pop_back();
    return llvmValue;
}

void CodegenCtx::pushCompiledDataType(const CompiledDataType & dataType) {
    mCompiledDataTypes.push_back(dataType);
}

CompiledDataType CodegenCtx::popCompiledDataType() {
    if (!mCompiledDataTypes.empty()) {
        CompiledDataType type = mCompiledDataTypes.back();
        mCompiledDataTypes.pop_back();
        return type;
    }
    
    return CompiledDataType(PrimitiveDataTypes::getUnknownDataType(), nullptr);
}

void CodegenCtx::handleDeferredCodegenCallbacks(std::vector<std::function<void ()>> & callbacks) {
    while (!callbacks.empty()) {
        const auto & callback = callbacks.back();
        callback();
        callbacks.pop_back();
    }
}

void CodegenCtx::setNodeEvaluatedDataType(const AST::ASTNode & astNode,
                                          std::unique_ptr<const DataType> & dataType)
{
    mNodeEvaluatedDataTypes[&astNode].reset(dataType.release());
}

const DataType * CodegenCtx::getNodeEvaluatedDataType(const AST::ASTNode & astNode) const {
    auto iter = mNodeEvaluatedDataTypes.find(&astNode);
    WC_GUARD(iter != mNodeEvaluatedDataTypes.end(), nullptr);
    return iter->second.get();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
