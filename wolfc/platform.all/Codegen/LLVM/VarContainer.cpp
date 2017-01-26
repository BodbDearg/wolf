#include "VarContainer.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "CodegenCtx.hpp"
#include "Lexer/WCToken.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

DataValue * VarContainer::createVar(CodegenCtx & ctx,
                                    const std::string & varName,
                                    const DataType & varType,
                                    llvm::Value & varValue,
                                    bool requiresLoad,
                                    const AST::ASTNode & varDeclaringNode)
{
    // If the variable already exists in this scope then creation fails and issue a compile error
    {
        auto iter = mVarValues.find(varName);
        
        if (iter != mVarValues.end()) {
            const DataValue & existingVal = iter->second;
            const Token & existingValStarTok = existingVal.declaringNode->getStartToken();
            
            ctx.error(varDeclaringNode,
                      "Attempting to redeclare variable '%s' more than once in the same scope! "
                      "Variable is already declared at line %zu, col %zu!",
                      varName.c_str(),
                      existingValStarTok.startLine + 1,
                      existingValStarTok.startCol + 1);
            
            return nullptr;
        }
    }
    
    // Make the data value:
    DataValue & dataValue = mVarValues[varName];
    dataValue.declaringNode = &varDeclaringNode;
    dataValue.requiresLoad = requiresLoad;
    dataValue.type = &varType;
    dataValue.value = &varValue;
    return &dataValue;
}

const DataValue * VarContainer::getVar(const char * varName) const {
    auto iter = mVarValues.find(varName);
    WC_GUARD(iter != mVarValues.end(), nullptr);
    return &iter->second;
}

const DataValue * VarContainer::getVar(const std::string & varName) const {
    auto iter = mVarValues.find(varName);
    WC_GUARD(iter != mVarValues.end(), nullptr);
    return &iter->second;
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
