#pragma once

#include "WCASTNode.hpp"
#include <memory>
#include <map>

namespace llvm {
    class LLVMContext;
    class Module;
}

WC_BEGIN_NAMESPACE

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
    
    llvm::LLVMContext &             mLLVMCtx;
    std::unique_ptr<llvm::Module>   mLLVMMod;
    DeclDefs *                      mDeclDefs = nullptr;
    
    /* A list of registered functions in the module. Generated during the compile phase. */
    std::map<std::string, Func*> mFuncs;
};

WC_END_NAMESPACE
