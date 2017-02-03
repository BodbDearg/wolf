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

WC_LLVM_BACKEND_BEGIN_NAMESPACE

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
     * If the value already exists then a compile error is emitted and the constant is given a '#2', 
     * '#3' etc. suffix in order to allow the value to be created anyway.
     */
    Value & createVal(CodegenCtx & ctx,
                      const std::string & name,
                      llvm::Value * llvmVal,
                      const CompiledDataType & compiledType,
                      bool requiresLoad,
                      const AST::ASTNode & declaringNode);
    
    /** 
     * Create a constant within this holder.
     * If the constant already exists then a compile error is emitted and the constant is given a '#2', 
     * '#3' etc. suffix in order to allow the constant to be created anyway.
     */
    Constant & createConst(CodegenCtx & ctx,
                           const std::string & name,
                           llvm::Constant * llvmConst,
                           const CompiledDataType & compiledType,
                           const AST::ASTNode & declaringNode);
    
    /**
     * Get a value within this holder. Returns null if not found within this scope.
     * Note: constants can also be retrieved as values too via this method.
     */
    const Value * getVal(const char * name) const;
    const Value * getVal(const std::string & name) const;
    Value * getVal(const char * name);
    Value * getVal(const std::string & name);
    
    /* Get a constant within this holder. Returns null if not found within this scope. */
    const Constant * getConst(const char * name) const;
    const Constant * getConst(const std::string & name) const;
    Constant * getConst(const char * name);
    Constant * getConst(const std::string & name);
    
private:
    /**
     * Compile check the given name for a value declaration is not taken.
     * Issues a compile error if the name is taken and changes the name to have a unique suffix such as 
     * '#2' or '#3' in the given output name string. If the name is valid then the given output name matches
     * the input name.
     */
    void compileCheckValueNameNotTaken(CodegenCtx & ctx,
                                       const std::string & name,
                                       std::string & outputUniqueName,
                                       const CompiledDataType & compiledType,
                                       const AST::ASTNode & declaringNode) const;
    
    /* The values in the container */
    std::map<std::string, Value, std::less<>> mValues;
    
    /* The constants in the container */
    std::map<std::string, Constant, std::less<>> mConstants;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
