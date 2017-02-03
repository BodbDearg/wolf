#include "ValHolder.hpp"

#include "AST/Nodes/ASTNode.hpp"
#include "CodegenCtx.hpp"
#include "DataType/DataType.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

Value & ValHolder::createVal(CodegenCtx & ctx,
                             const std::string & name,
                             llvm::Value * llvmVal,
                             const CompiledDataType & compiledType,
                             bool requiresLoad,
                             const AST::ASTNode & declaringNode)
{
    // Start filling in some parts of the value
    Value value;
    value.mLLVMVal = llvmVal;
    value.mCompiledType = compiledType;
    value.mRequiresLoad = requiresLoad;
    value.mDeclaringNode = &declaringNode;
    
    // Make sure the name is not taken, issue a compile error if so.
    // This will also set the unique name of the value.
    compileCheckValueNameNotTaken(ctx,
                                  name,
                                  value.mName,
                                  compiledType,
                                  declaringNode);
    
    // Store the value with the unique name:
    return mValues[value.mName] = std::move(value);
}

Constant & ValHolder::createConst(CodegenCtx & ctx,
                                  const std::string & name,
                                  llvm::Constant * llvmConst,
                                  const CompiledDataType & compiledType,
                                  const AST::ASTNode & declaringNode)
{
    // Start filling in some parts of the value.
    // Note that constants are a form of value too, so we register constants as values as well.
    Value value;
    value.mLLVMVal = llvmConst;
    value.mCompiledType = compiledType;
    value.mRequiresLoad = false;                // Constants never require a load
    value.mDeclaringNode = &declaringNode;
    
    // Make sure the name is not taken, issue a compile error if so.
    // This will also set the unique name of the value.
    // We can also use this unique name for the constant and guarantee it's uniqueness, since values
    // are registered as constants too.
    compileCheckValueNameNotTaken(ctx,
                                  name,
                                  value.mName,
                                  compiledType,
                                  declaringNode);
    
    // Make the constant:
    Constant constant;
    constant.mName = value.mName;
    constant.mLLVMConst = llvmConst;
    constant.mCompiledType = compiledType;
    constant.mDeclaringNode = &declaringNode;
    
    // Save the value:
    mValues[value.mName] = std::move(value);
    
    // The returned value is still the constant however
    return mConstants[constant.mName] = std::move(constant);
}

const Value * ValHolder::getVal(const char * name) const {
    auto iter = mValues.find(name);
    WC_GUARD(iter != mValues.end(), nullptr);
    return &iter->second;
}

const Value * ValHolder::getVal(const std::string & name) const {
    auto iter = mValues.find(name);
    WC_GUARD(iter != mValues.end(), nullptr);
    return &iter->second;
}

Value * ValHolder::getVal(const char * name) {
    auto iter = mValues.find(name);
    WC_GUARD(iter != mValues.end(), nullptr);
    return &iter->second;
}

Value * ValHolder::getVal(const std::string & name) {
    auto iter = mValues.find(name);
    WC_GUARD(iter != mValues.end(), nullptr);
    return &iter->second;
}

const Constant * ValHolder::getConst(const char * name) const {
    auto iter = mConstants.find(name);
    WC_GUARD(iter != mConstants.end(), nullptr);
    return &iter->second;
}

const Constant * ValHolder::getConst(const std::string & name) const {
    auto iter = mConstants.find(name);
    WC_GUARD(iter != mConstants.end(), nullptr);
    return &iter->second;
}

Constant * ValHolder::getConst(const char * name) {
    auto iter = mConstants.find(name);
    WC_GUARD(iter != mConstants.end(), nullptr);
    return &iter->second;
}

Constant * ValHolder::getConst(const std::string & name) {
    auto iter = mConstants.find(name);
    WC_GUARD(iter != mConstants.end(), nullptr);
    return &iter->second;
}

void ValHolder::compileCheckValueNameNotTaken(CodegenCtx & ctx,
                                              const std::string & name,
                                              std::string & outputUniqueName,
                                              const CompiledDataType & compiledType,
                                              const AST::ASTNode & declaringNode) const
{
    // Just check the ordinary values map, constants are registered as values too
    auto iter = mValues.find(name);
    
    if (iter == mValues.end()) {
        // Name is unique, save the output name and bail out, our work is done
        outputUniqueName = name;
        return;
    }
    
    // Okay we have a duplicate name error, log it:
    {
        const Value & otherVal = iter->second;
        const Token & otherValStartTok = otherVal.mDeclaringNode->getStartToken();
        
        ctx.error(declaringNode,
                  "Duplicate declaration named '%s'! Declaration of type '%s' has already been declared "
                  "at line %zu, col %zu as a declaration of type '%s'!",
                  name.c_str(),
                  compiledType.getDataType().name().c_str(),
                  otherValStartTok.startLine + 1,
                  otherValStartTok.startCol + 1,
                  otherVal.mCompiledType.getDataType().name().c_str());
    }
    
    // Okay, search for a unique name.
    // Future improvement: We could do a binary search here to speed this up, but we're probably not going
    // to have too many duplicates in most code (I hope)
    std::string uniqueName;
    uniqueName.reserve(name.size() + 16);
    uniqueName += name;
    size_t duplicateNum = 2;
    
    while (true) {
        uniqueName += std::to_string(duplicateNum);
        
        if (!getVal(uniqueName.c_str())) {
            // Found a unique name, stop the loop
            outputUniqueName = uniqueName;
            break;
        }
        
        ++duplicateNum;     // This name is taken also, try another
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
