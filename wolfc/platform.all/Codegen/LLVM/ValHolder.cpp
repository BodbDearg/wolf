#include "ValHolder.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

const Value * ValHolder::createVal(CodegenCtx & ctx,
                                   const std::string & name,
                                   const DataType & type,
                                   llvm::Value & llvmValue,
                                   bool requiresLoad,
                                   const AST::ASTNode & declaringNode)
{
    // Make sure the name is not taken
    WC_GUARD(compileCheckValueNameNotTaken(ctx, name, type, declaringNode), nullptr);
    
    // Make the value:
    Value & value = mValues[name];
    value.mType = &type;
    value.mLLVMValue = &llvmValue;
    value.mRequiresLoad = requiresLoad;
    value.mDeclaringNode = &declaringNode;
    return &value;
}

const Constant * ValHolder::createConst(CodegenCtx & ctx,
                                        const std::string & name,
                                        const DataType & type,
                                        llvm::Constant & llvmConstant,
                                        const AST::ASTNode & declaringNode)
{
    // Make sure the name is not taken
    WC_GUARD(compileCheckValueNameNotTaken(ctx, name, type, declaringNode), nullptr);
    
    // Make the constant:
    Constant & constant = mConstants[name];
    constant.mType = &type;
    constant.mLLVMConstant = &llvmConstant;
    constant.mDeclaringNode = &declaringNode;
    
    // Register the constant as an ordinary value too, can be accessed in that way
    Value & value = mValues[name];
    value.mType = &type;
    value.mLLVMValue = &llvmConstant;
    value.mRequiresLoad = false;                // Constants never require a load
    value.mDeclaringNode = &declaringNode;
    
    // The returned value is still the constant however
    return &constant;
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

bool ValHolder::compileCheckValueNameNotTaken(CodegenCtx & ctx,
                                              const std::string & name,
                                              const DataType & type,
                                              const AST::ASTNode & declaringNode) const
{
    // Just check the ordinary values map, constants are registered as values too
    auto iter = mValues.find(name);
    WC_GUARD(iter != mValues.end(), true);
    const Value & otherVal = iter->second;
    const Token & otherValStartTok = otherVal.mDeclaringNode->getStartToken();
    
    // We have an error, log it:
    ctx.error(declaringNode,
              "Duplicate declaration named '%s'! Declaration of type '%s' has already been declared "
              "at line %zu, col %zu as a declaration of type '%s'!",
              name.c_str(),
              type.name().c_str(),
              otherValStartTok.startLine + 1,
              otherValStartTok.startCol + 1,
              otherVal.mType->name().c_str());
    
    // False for failure
    return false;
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
