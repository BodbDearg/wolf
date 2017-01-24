#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
    #include <llvm/IR/Module.h>
    #include <string>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
    class Func;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class Function;

/* Class holding the context for code generation */
class CodegenCtx {
public:
    CodegenCtx();
    
    ~CodegenCtx();
    
    /* Tells if there are errors in the codegen context */
    bool hasErrors() const;
    
    /* Get the error messages stored in the parse context */
    inline const auto & getErrorMsgs() const {
        return mErrorMsgs;
    }    
    
    /* Tells if there are warnings in the codegen context */
    bool hasWarnings() const;
    
    /* Get the warning messages stored in the parse context */
    inline const auto & getWarningMsgs() const {
        return mWarningMsgs;
    }    
    
    /**
     * Emit an error message and save to the list of error messages in the codegen context.
     * The line and column info for the error source include the range of the specified AST node.
     * If the source node is not specified, the AST node stack will be used for location info (if possible).
     *
     * Emitting an error will put the codegen context into an error state.
     */
    void error(const AST::ASTNode & atNode, const char * msgFmtStr, ...);
    void error(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs);
    void error(const char * msgFmtStr, ...);
    void error(const char * msgFmtStr, std::va_list msgFmtStrArgs);
    
    /**
     * Emit a warning message and save to the list of warning messages in the parse context.
     * The line and column info for the warning source include the range of the specified AST node.
     * If the source node is not specified, the AST node stack will be used for location info (if possible).
     */
    void warning(const AST::ASTNode & atNode, const char * msgFmtStr, ...);
    void warning(const AST::ASTNode & atNode, const char * msgFmtStr, std::va_list msgFmtStrArgs);
    void warning(const char * msgFmtStr, ...);
    void warning(const char * msgFmtStr, std::va_list msgFmtStrArgs);
    
    /* Push the current codegen basic block to the stack and save for later popping. */
    void pushInsertBlock();
    
    /**
     * Pop a previous insert block from the stack.
     * There MUST be an insert block on the stack, assumes this is the case (undefined behavior otherwise).
     */
    void popInsertBlock();

    /**
     * Dump the IR code for the module to stdout.
     * Returns false if this is not possible.
     */
    bool dumpIRCodeToStdout();
    
    /* Register the given function in the module. Issues a compile error if already registered. */
    bool registerModuleFunc(const AST::Func & astNode);
    
    /* Retrieve the function of the specified name in the module. Returns nullptr if not found. */
    Function * getModuleFunc(const char * name);
    Function * getModuleFunc(const std::string & name);
    
    /**
     * Push/pop the current node being visited during code generation from the node visitation stack.
     * The behavior of pop is undefined if the visited node stack is empty.
     */
    void pushASTNode(const AST::ASTNode & node);
    void popASTNode();
    
    inline const std::vector<const AST::ASTNode*> getASTNodeStack() const {
        return mASTNodeStack;
    }
    
    /**
     * Push/pop an llvm::Type object to the stack.
     * If popping and the stack is emtpy, a null pointer will be returned.
     */
    void pushLLVMType(llvm::Type & llvmType);
    llvm::Type * popLLVMType();
    
    /**
     * Push/pop an llvm::Value object to the stack.
     * If popping and the stack is emtpy, a null pointer will be returned.
     */
    void pushLLVMValue(llvm::Value & llvmValue);
    llvm::Value * popLLVMValue();
    
    /**
     * Handles the given list of deferred codegen callbacks.
     * Calls each callback and drains the list.
     */
    void handleDeferredCodegenCallbacks(std::vector<std::function<void ()>> & callbacks);
    
    /* The LLVM context */
    llvm::LLVMContext mLLVMCtx;
    
    /* The llvm IR builder object. Used for creating most instructions. */
    llvm::IRBuilder<> mIRBuilder;
    
    /* The LLVM module */
    std::unique_ptr<llvm::Module> mLLVMModule;
    
    /* The current Function we are visiting during codegen. */
    Function * mCurFunction = nullptr;
    
    /**
     * Deferred codegen callbacks called after visiting AST::Module.
     * Used to generate the inner bodies of functions and allows the functions to be declared
     * and used in any order (unlike C).
     */
    std::vector<std::function<void ()>> mDeferredCgCallbacks_Module;
    
private:
    WC_DISALLOW_COPY_AND_ASSIGN(CodegenCtx)
    
    /* The stack of AST nodes being visited */
    std::vector<const AST::ASTNode*> mASTNodeStack;
    
    /* A stack of llvm types created during codegen. Used for communication, like the stack in LUA. */
    std::vector<llvm::Type*> mLLVMTypes;
    
    /* A stack of values types created during codegen. Used for communication, like the stack in LUA. */
    std::vector<llvm::Value*> mLLVMValues;
    
    /* A list of error messages emitted during parsing */
    std::vector<std::string> mErrorMsgs;
    
    /* A list of warning messages emitted during parsing */
    std::vector<std::string> mWarningMsgs;
    
#warning STILL required?
    /* A stack of code insert blocks pushed/saved for later restoring. */
    std::vector<llvm::BasicBlock*> mInsertBlockStack;
    
    /* A list of registered functions in the module by name. */
    std::map<std::string, std::unique_ptr<Function>> mFuncs;
};

/* A helper RAII object which pushes and pops a node from the given codegen context. */
class CodegenCtxPushASTNode {
public:
    CodegenCtxPushASTNode(const AST::ASTNode & astNode, CodegenCtx & codegenCtx) : mCodegenCtx(codegenCtx) {
        mCodegenCtx.pushASTNode(astNode);
    }
    
    ~CodegenCtxPushASTNode() {
        mCodegenCtx.popASTNode();
    }
    
private:
    CodegenCtx & mCodegenCtx;
};

/* A helper macro for using the 'CodegenCtxPushASTNode' object within a code generator */
#define WC_CODEGEN_RECORD_VISITED_NODE()\
    CodegenCtxPushASTNode pushASTNode(astNode, mCtx)

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
