#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
    #include <llvm/IR/Module.h>
    #include <string>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class LLVMContext;
    class Module;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Class holding the context for code generation */
class CodegenCtx {
    /* Creates the codegen context */
    CodegenCtx();
    
    /* Tells if there are errors in the codegen context */
    bool hasErrors() const;
    
    /* Tells if there are warnings in the codegen context */
    bool hasWarnings() const;
    
    /**
     * Emit an error message and save to the list of error messages in the codegen context.
     * The line and column info for the error source include the range of the specified AST node.
     *
     * Emitting an error will put the codegen context into an error state.
     */
    void error(const AST::ASTNode & atNode, const char * msgFmtStr, ...);
    void error(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs);
    
    /**
     * Emit a warning message and save to the list of warning messages in the parse context.
     * The line and column info for the warning source include the range of the specified AST node.
     */
    void warning(const AST::ASTNode & atNode, const char * msgFmtStr, ...);
    void warning(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs);
    
    /* Push the current codegen basic block to the stack and save for later popping. */
    void pushInsertBlock();
    
    /**
     * Pop a previous insert block from the stack.
     * There MUST be an insert block on the stack, assumes this is the case (undefined behavior otherwise).
     */
    void popInsertBlock();
    
    /* The LLVM context */
    llvm::LLVMContext llvmCtx;
    
    /* The llvm IR builder object. Used for creating most instructions. */
    llvm::IRBuilder<> irBuilder;
    
    /* The LLVM module */
    std::unique_ptr<llvm::Module> llvmModule;
    
#warning FIXME - Codegen
#if 0
    /**
     * Type for a deferred code generation lambda callback.
     * The parameter given is this code generation context and the return value is whether the code generated
     * successfully or not.
     */
    typedef std::function<bool (CodegenCtx & cgCtx)> DeferredCodegenCallback;

    /**
     * A list of callbacks that will be called in order to perform deferred code generation.
     *
     * For certain parts of the code generation, we must break our work into stages and defer
     * parts of the code generation until later. For example statements that modify normal program 
     * flow such as 'break' and 'next' need to have their code generation deferred because they
     * need to refer to basic blocks around them that may not exist at the time code generation is invoked.
     * Similiarly so, we must define all functions in a module before doing code generation for the function
     * bodies themselves so that we don't have the C/C++ limitation of having to define or forward declare
     * a function within a module before it is used.
     *
     * Note that deferred generation callbacks may themselves generate further deferred code generation
     * requests, allowing for a multi-pass complimation pipeline.
     *
     * Deferred code generation is perfomed in FIFO (queue) order, with the first item in the
     * list being generated first.
     */
    std::list<DeferredCodegenCallback> deferredCodegenCallbacks;
#endif
    
private:
    /* A list of error messages emitted during parsing */
    std::vector<std::string> mErrorMsgs;
    
    /* A list of warning messages emitted during parsing */
    std::vector<std::string> mWarningMsgs;
    
    /* A stack of code insert blocks pushed/saved for later restoring. */
    std::vector<llvm::BasicBlock*> mInsertBlockStack;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
