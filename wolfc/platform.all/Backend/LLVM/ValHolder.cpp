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
                             const AST::ASTNode & declaringNode,
                             bool noDuplicateNameCheck)
{
    // Start filling in some parts of the value
    Value value;
    value.mLLVMVal = llvmVal;
    value.mCompiledType = compiledType;
    value.mRequiresLoad = requiresLoad;
    value.mDeclaringNode = &declaringNode;
    
    // Unless otherwise specified, make sure the name is not taken and issue a compile error if so.
    // This will also set the unique name of the value.
    if (!noDuplicateNameCheck) {
        compileCheckNameNotTaken(ctx,
                                 name,
                                 value.mName,
                                 compiledType,
                                 declaringNode);
    }
    else {
        value.mName = name;
    }
    
    // Store the value with the unique name:
    return mValues[value.mName] = std::move(value);
}

Constant & ValHolder::createConst(CodegenCtx & ctx,
                                  const std::string & name,
                                  llvm::Constant * llvmConst,
                                  const CompiledDataType & compiledType,
                                  const AST::ASTNode & declaringNode,
                                  bool noDuplicateNameCheck)
{
    // Start filling in some parts of the constant
    Constant constant;
    constant.mLLVMConst = llvmConst;
    constant.mCompiledType = compiledType;
    constant.mDeclaringNode = &declaringNode;
    
    // Unless otherwise specified, make sure the name is not taken and issue a compile error if so.
    // This will also set the unique name of the value.
    if (!noDuplicateNameCheck) {
        compileCheckNameNotTaken(ctx,
                                 name,
                                 constant.mName,
                                 compiledType,
                                 declaringNode);
    }
    else {
        constant.mName = name;
    }
    
    // Store the constant with the unique name:
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

bool ValHolder::compileCheckNameNotTaken(CodegenCtx & ctx,
                                         const std::string & name,
                                         std::string & outputUniqueName,
                                         const CompiledDataType & compiledType,
                                         const AST::ASTNode & declaringNode) const
{
    // Check to see if it exists in the set of values
    bool duplicateNameError = false;
    
    {
        auto iter = mValues.find(name);
        
        if (iter != mValues.end()) {
            // Okay we have a duplicate name error, log it if not in silent mode:
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
            
            // We found an error
            duplicateNameError = true;
        }
    }
    
    // Check to see if it exists in the set of constants
    if (!duplicateNameError) {
        auto iter = mConstants.find(name);
        
        if (iter != mConstants.end()) {
            // Okay we have a duplicate name error, log it if not in silent mode:
            const Constant & otherConst = iter->second;
            const Token & otherConstStartTok = otherConst.mDeclaringNode->getStartToken();
            
            ctx.error(declaringNode,
                      "Duplicate declaration named '%s'! Declaration of type '%s' has already been declared "
                      "at line %zu, col %zu as a declaration of type '%s'!",
                      name.c_str(),
                      compiledType.getDataType().name().c_str(),
                      otherConstStartTok.startLine + 1,
                      otherConstStartTok.startCol + 1,
                      otherConst.mCompiledType.getDataType().name().c_str());
            
            // We found an error
            duplicateNameError = true;
        }
    }
    
    // If we are good then save the output name and bail:
    if (!duplicateNameError) {
        outputUniqueName = name;
        return true;
    }
    
    // Okay, search for a unique name.
    // Future improvement: We could do a binary search here to speed this up, but we're probably not going
    // to have too many duplicates in most code (I hope)
    std::string uniqueName;
    uniqueName.reserve(name.size() + 16);
    uniqueName += name;
    size_t duplicateNum = 2;
    
    while (true) {
        // Try this name:
        uniqueName += '#';
        uniqueName += std::to_string(duplicateNum);
        
        // If we found a unique name, stop the loop...
        if (!getVal(uniqueName.c_str()) && !getConst(uniqueName.c_str())) {
            outputUniqueName = uniqueName;
            break;
        }
        
        // This name is taken also, try another.
        ++duplicateNum;
        
        // Need to reset this:
        uniqueName.resize(name.size());
    }
    
    // An error happened
    return false;
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
