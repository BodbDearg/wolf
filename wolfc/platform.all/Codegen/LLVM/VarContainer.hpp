#pragma once

#include "DataValue.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <map>
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenCtx;

/**
 * Class which allows variables to be registered and provides access to the variables 
 * during code generation.
 */
class VarContainer {
public:
    VarContainer() = default;
    
    /** 
     * Create a variable within this scope.
     * If the variable already exists then creation fails and null is returned.
     * If creation fails a compile error is also emitted.
     */
    DataValue * createVar(CodegenCtx & ctx,
                          const std::string & varName,
                          const DataType & varType,
                          llvm::Value & varValue,
                          bool requiresLoad,
                          const AST::ASTNode & varDeclaringNode);
    
    /* Get a variable within this scope. Returns null if not found within this scope. */
    const DataValue * getVar(const char * varName) const;
    const DataValue * getVar(const std::string & varName) const;
    
private:
    /* The variables in the container */
    std::map<std::string, DataValue, std::less<>> mVarValues;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
