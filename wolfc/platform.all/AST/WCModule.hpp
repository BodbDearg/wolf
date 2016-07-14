#pragma once

#include "WCASTNode.hpp"
#include <memory>

namespace llvm {
    class LLVMContext;
    class Module;
}

WC_BEGIN_NAMESPACE

class DeclDefs;
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
    
    llvm::LLVMContext &             mLLVMCtx;
    std::unique_ptr<llvm::Module>   mLLVMMod;
    DeclDefs *                      mDeclDefs = nullptr;
};

WC_END_NAMESPACE
