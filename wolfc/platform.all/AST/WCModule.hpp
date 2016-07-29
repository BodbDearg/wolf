#pragma once

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"
#include "WCDataValue.hpp"
#include <memory>
#include <map>

namespace llvm {
    class Constant;
    class LLVMContext;
    class Module;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class DeclDefs;
class Func;
class LinearAlloc;

/* 
Module:
    DeclDefs <EOF>
*/
class Module : public ASTNode {
public:
    Module(llvm::LLVMContext & llvmCtx);
    
    ~Module();
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    // TODO: change this to return a new module, similar to other ASTNodes
    /* Parse the code for the module from the given token array */
    bool parseCode(const Token * tokenList, LinearAlloc & alloc);
    
    /* Generates the code for the module. Parsing code must have succeeded. */
    bool generateCode();
    
    /* Tells if the code was generated ok */
    bool wasCodeGeneratedOk();
    
    /* Dump the generated LLVM IR code to stdout */
    void dumpIRCodeToStdout();
    
    /** 
     * Register a given function with the module.
     * Returns false on failure if the function is already registered. 
     */
    bool registerFunc(Func & func);
    
    /* Lookup a module function by name. Returns nullptr if not found. */
    Func * getFunc(const std::string & name) const;
    
    /**
     * Create a variable within this scope.
     * If the variable already exists then creation fails and null is returned.
     */
    DataValue * createVar(const char * varName,
                          DataType & dataType,
                          llvm::Constant * initializer,
                          CodegenCtx & cgCtx,
                          ASTNode & callingNode);
    
    /**
     * Get a variable within this scope or a parent scope. Does not create if it does not exist.
     * Will search parent scopes if not found within this scope.
     */
    DataValue * getVar(const char * varName) const;
    
    /* The LLVM context */
    llvm::LLVMContext & mLLVMCtx;
    
    /* The LLVM module */
    std::unique_ptr<llvm::Module> mLLVMModule;
    
    /* All declarations and definitions in the module/ */
    DeclDefs * mDeclDefs = nullptr;
    
    /* A list of registered functions in the module. Generated during the compile phase. */
    std::map<std::string, Func*> mFuncs;
    
    /* A list of registered global variables in the module. Generated during the compile phase. */
    std::map<const char*, DataValue, CStrComparator> mVarValues;
};

WC_END_NAMESPACE
