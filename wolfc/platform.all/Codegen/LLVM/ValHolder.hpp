#pragma once

#include "Value.hpp"

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
 * Class which allows values, constant and non constant to be registered and provides access 
 * to the values during code generation. Values can be variables, constants or functions.
 */
class ValHolder {
public:
    ValHolder() = default;
    
    /** 
     * Create a value within this holder.
     * If the value already exists then creation fails and null is returned.
     * If creation fails a compile error is also emitted.
     */
    const Value * createVal(CodegenCtx & ctx,
                            const std::string & name,
                            const DataType & type,
                            llvm::Value & llvmValue,
                            bool requiresLoad,
                            const AST::ASTNode & declaringNode);
    
    /* Get a value within this holder. Returns null if not found within this scope. */
    const Value * getVal(const char * name) const;
    const Value * getVal(const std::string & name) const;
    
private:
    /* The values in the container */
    std::map<std::string, Value, std::less<>> mValues;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
