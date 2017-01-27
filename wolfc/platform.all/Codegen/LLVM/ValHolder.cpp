#include "ValHolder.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "CodegenCtx.hpp"
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
    // If the variable already exists in this scope then creation fails and issue a compile error
    {
        auto iter = mValues.find(name);
        
        if (iter != mValues.end()) {
            const Value & value = iter->second;
            const Token & valueStartTok = value.mDeclaringNode->getStartToken();
            
            ctx.error(declaringNode,
                      "Attempting to redeclare variable '%s' more than once in the same scope! "
                      "Variable is already declared at line %zu, col %zu!",
                      name.c_str(),
                      valueStartTok.startLine + 1,
                      valueStartTok.startCol + 1);
            
            return nullptr;
        }
    }
    
    // Make the data value:
    Value & value = mValues[name];
    value.mDeclaringNode = &declaringNode;
    value.mRequiresLoad = requiresLoad;
    value.mType = &type;
    value.mLLVMValue = &llvmValue;
    return &value;
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

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
