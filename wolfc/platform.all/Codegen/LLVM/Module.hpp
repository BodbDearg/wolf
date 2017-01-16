#pragma once

#include "WCAssert.hpp"
#include "WCMacros.hpp"

namespace llvm {
    class LLVMContext;
    class Module;
}

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <memory>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace AST {
    class Module;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Class responsible for generating code with LLVM for a module */
class Module final {
public:
    Module(AST::Module & astModule);
    
    ~Module();
    
    /* Tells if the code was generated ok */
    bool wasCodeGeneratedOk();
    
    /**
     * Dump the generated LLVM IR code to stdout.
     * @pre This should only be called if wasCodeGeneratedOk() returns true.
     */
    void dumpIRCodeToStdout();
    
    /* Get the LLVM module for this module */
    inline llvm::Module * getLLVMModule() const {
        return mLLVMModule.get();
    }
    
    inline llvm::Module & getLLVMModuleRef() const {
        WC_ASSERT(mLLVMModule.get());
        return *mLLVMModule.get();
    }
    
private:
    /* Do the code generation for the module. Returns false if failed. */
    bool doCodegen();
    
    /**
     * Declare C standard library functions with the module.
     *
     * @pre llvm context must have been created.
     * @pre llvm module must have been created.
     */
    void declareCStdLibFuncsInModule();
    
    /* The AST for the module */
    AST::Module & mASTModule;
    
    /* The LLVM context */
    std::unique_ptr<llvm::LLVMContext> mLLVMCtx;
    
    /* The LLVM module */
    std::unique_ptr<llvm::Module> mLLVMModule;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
