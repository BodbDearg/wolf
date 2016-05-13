#pragma once

#include "WCASTNode.hpp"
#include <memory>

namespace llvm {
    class LLVMContext;
    class Module;
}

WC_BEGIN_NAMESPACE

class Scope;

/* 
Module:
    Scope <EOF>
*/
class Module : public ASTNode {
public:
    Module(llvm::LLVMContext & llvmCtx);
    
    ~Module();
    
    /* Parse the code for the module from the given token array */
    bool parseCode(const Token * tokenList);
    
    /* Generates the code for the module. Parsing code must have succeeded. */
    bool generateCode();
    
    /* Tells if the code was generated ok */
    bool wasCodeGeneratedOk();
    
    /* Dump the generated LLVM IR code to stdout */
    void dumpIRCodeToStdout();
    
    llvm::LLVMContext &             mLLVMCtx;
    std::unique_ptr<llvm::Module>   mLLVMMod;
    std::unique_ptr<Scope>          mScope;
};

WC_END_NAMESPACE
