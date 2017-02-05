#include "CodegenCtx.hpp"

#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/IRepeatableStmnt.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/InvalidDataType.hpp"
#include "Lexer/Token.hpp"
#include "RepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenCtx::CodegenCtx() :
    mLLVMCtx(),
    mIRBuilder(mLLVMCtx)
{
    // Reserving some room for these things
    mASTNodeStack.reserve(4096);
    mScopeStack.reserve(128);
    mValues.reserve(32);
    mConstants.reserve(32);
    mCompiledDataTypes.reserve(32);
    mErrorMsgs.reserve(256);
    mWarningMsgs.reserve(256);
    mDeferredCgCallbacks_Module.reserve(1024);
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

RepeatableStmnt & CodegenCtx::getRepeatableStmntForNode(const AST::ASTNode & astNode,
                                                        const AST::IRepeatableStmnt & astNodeAsRepeatableStmnt)
{
    // These AST node references should refer to the same thing
    WC_ASSERT(dynamic_cast<const AST::IRepeatableStmnt*>(&astNode) == &astNodeAsRepeatableStmnt);
    auto iter = mRepeatableStmnts.find(&astNodeAsRepeatableStmnt);
    
    // If we find an existing data structure for this statement then just return it
    if (iter != mRepeatableStmnts.end()) {
        return *iter->second;
    }
    
    // Otherwise we need to make one
    auto & ptr = mRepeatableStmnts[&astNodeAsRepeatableStmnt];
    ptr.reset(new RepeatableStmnt(astNode, astNodeAsRepeatableStmnt));
    return *ptr.get();
}

RepeatableStmnt * CodegenCtx::getCurrentRepeatableStmnt() const {
    // Run through the AST node stack and search for the current repeatable statement
    ssize_t numNodes = static_cast<ssize_t>(mASTNodeStack.size());
    
    for (ssize_t i = numNodes - 1; i >= 0; --i) {
        const AST::ASTNode * astNode = mASTNodeStack[static_cast<size_t>(i)];
        const AST::IRepeatableStmnt * astRepeatableStmt = dynamic_cast<const AST::IRepeatableStmnt*>(astNode);
        
        if (astRepeatableStmt) {
            auto iter = mRepeatableStmnts.find(astRepeatableStmt);
            
            if (iter != mRepeatableStmnts.end()) {
                return iter->second.get();
            }
        }
    }
    
    // Couldn't find any:
    return nullptr;
}

void CodegenCtx::pushASTNode(const AST::ASTNode & node) {
    mASTNodeStack.push_back(&node);
}

void CodegenCtx::popASTNode() {
    WC_ASSERT(!mASTNodeStack.empty());
    mASTNodeStack.pop_back();
}

void CodegenCtx::pushScope(const AST::Scope & node) {
    mScopeStack.push_back(&node);
}

void CodegenCtx::popScope() {
    WC_ASSERT(!mScopeStack.empty());
    mScopeStack.pop_back();
}

const AST::Scope * CodegenCtx::getCurrentScope() const {
    if (!mScopeStack.empty()) {
        return mScopeStack.back();
    }
    
    return nullptr;
}

void CodegenCtx::pushValue(const Value & value) {
    mValues.push_back(value);
}

Value CodegenCtx::popValue() {
    if (mValues.empty()) {
        return Value();
    }
    
    Value value = mValues.back();
    mValues.pop_back();
    return value;
}

void CodegenCtx::pushConstant(const Constant & constant) {
    mConstants.push_back(constant);
}

Constant CodegenCtx::popConstant() {
    if (mConstants.empty()) {
        return Constant();
    }
    
    Constant constant = mConstants.back();
    mConstants.pop_back();
    return constant;
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
    
    return CompiledDataType(PrimitiveDataTypes::getInvalidDataType(), nullptr);
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
    // This can't be set twice for the same node:
    auto & evaluatedType = mNodeEvaluatedDataTypes[&astNode];
    WC_GUARD_ASSERT(!evaluatedType.get());
    
    // Transer ownership of the data type object
    evaluatedType.reset(dataType.release());
}

const DataType * CodegenCtx::getNodeEvaluatedDataType(const AST::ASTNode & astNode) const {
    auto iter = mNodeEvaluatedDataTypes.find(&astNode);
    WC_GUARD(iter != mNodeEvaluatedDataTypes.end(), nullptr);
    return iter->second.get();
}

ValHolder & CodegenCtx::getScopeValHolder(const AST::Scope & scope) {
    return mScopeValHolders[&scope];
}

const Value * CodegenCtx::lookupValueByName(const char * name) const {
    // Search through the scope stack first for the value, start with the
    // innermost scope and work our way down the stack:
    WC_ASSERT(name);
    ssize_t scopeStackSize = static_cast<ssize_t>(mScopeStack.size());
    
    for (ssize_t i = scopeStackSize - 1; i >= 0; --i) {
        // Get this scope in the stack and see if there is a value holder declared for this scope.
        // If there isn't a value holder then just skip this scope and move search onto the next one:
        const AST::Scope * scope = mScopeStack[static_cast<size_t>(i)];
        WC_ASSERT(scope);
        auto iter = mScopeValHolders.find(scope);
        
        if (iter == mScopeValHolders.end()) {
            continue;
        }
        
        // Get the var holder for the scope and lookup the value in that holder.
        // If found, then return the value:
        const ValHolder & varHolder = iter->second;
        const Value * value = varHolder.getVal(name);
        
        if (value) {
            return value;
        }
    }
    
    // Value not found in any scope. Search in the global module scope instead and return the
    // result of that particular search:
    return mModuleValHolder.getVal(name);
}

const Constant * CodegenCtx::lookupConstantByName(const char * name) const {
    // Search through the scope stack first for the value, start with the
    // innermost scope and work our way down the stack:
    WC_ASSERT(name);
    ssize_t scopeStackSize = static_cast<ssize_t>(mScopeStack.size());
    
    for (ssize_t i = scopeStackSize - 1; i >= 0; --i) {
        // Get this scope in the stack and see if there is a value holder declared for this scope.
        // If there isn't a value holder then just skip this scope and move search onto the next one:
        const AST::Scope * scope = mScopeStack[static_cast<size_t>(i)];
        WC_ASSERT(scope);
        auto iter = mScopeValHolders.find(scope);
        
        if (iter == mScopeValHolders.end()) {
            continue;
        }
        
        // Get the var holder for the scope and lookup the value in that holder.
        // If found, then return the value:
        const ValHolder & varHolder = iter->second;
        const Constant * constant = varHolder.getConst(name);
        
        if (constant) {
            return constant;
        }
    }
    
    // Value not found in any scope. Search in the global module scope instead and return the
    // result of that particular search:
    return mModuleValHolder.getConst(name);
}

CodegenCtxPushASTNode::CodegenCtxPushASTNode(const AST::ASTNode & node, CodegenCtx & ctx) : mCtx(ctx) {
    mCtx.pushASTNode(node);
}
    
CodegenCtxPushASTNode::~CodegenCtxPushASTNode() {
    mCtx.popASTNode();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
