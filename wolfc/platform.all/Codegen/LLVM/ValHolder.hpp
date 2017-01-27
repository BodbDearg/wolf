#pragma once

#include "Constant.hpp"
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
    
    /** 
     * Create a constant within this holder.
     * If the constant already exists then creation fails and null is returned.
     * If creation fails a compile error is also emitted.
     */
    const Constant * createConst(CodegenCtx & ctx,
                                 const std::string & name,
                                 const DataType & type,
                                 llvm::Constant & llvmConstant,
                                 const AST::ASTNode & declaringNode);
    
    /**
     * Get a value within this holder. Returns null if not found within this scope.
     * Note: constants can also be retrieved as values too via this method.
     */
    const Value * getVal(const char * name) const;
    const Value * getVal(const std::string & name) const;
    
    /* Get a constant within this holder. Returns null if not found within this scope. */
    const Constant * getConst(const char * name) const;
    const Constant * getConst(const std::string & name) const;
    
private:
    /**
     * Compile check the given value declaration name is not taken.
     * Returns false and issues a compile error if the name is taken.
     */
    bool compileCheckValueNameNotTaken(CodegenCtx & ctx,
                                       const std::string & name,
                                       const DataType & type,
                                       const AST::ASTNode & declaringNode) const;
    
    /* The values in the container */
    std::map<std::string, Value, std::less<>> mValues;
    
    /* The constants in the container */
    std::map<std::string, Constant, std::less<>> mConstants;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
