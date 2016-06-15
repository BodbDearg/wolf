#include "WCModule.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIDeferredCodegenStmnt.hpp"
#include "WCLinearAlloc.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

Module::Module(llvm::LLVMContext & llvmCtx) : mLLVMCtx(llvmCtx) {
    WC_EMPTY_FUNC_BODY();
}

Module::~Module() {
    // Defined here so callee doesn't need to know details of objects destroyed by std::unique_ptr...
    WC_EMPTY_FUNC_BODY();
}

const Token & Module::getStartToken() const {
    WC_ASSERT(mScope);
    return mScope->getStartToken();
}

const Token & Module::getEndToken() const {
    WC_ASSERT(mScope);
    return mScope->getEndToken();
}

bool Module::parseCode(const Token * tokenList, LinearAlloc & alloc) {
    mScope = Scope::parse(tokenList, alloc);
    WC_GUARD(mScope, false);
    mScope->mParent = this;
    
    if (tokenList->type != TokenType::kEOF) {
        parseError(*tokenList, "Expected EOF after scope!");
        mScope = nullptr;
        return false;
    }
    
    return true;
}

bool Module::generateCode() {
    // Clear out previous code and check we parsed ok
    mLLVMMod.reset();
    
    if (!mScope) {
        compileError("Can't generate code, parsing was not successful!");
        return false;
    }
    
    // TODO: alloc this memory with the linear allocator
    // Create module
    mLLVMMod.reset(new llvm::Module("WolfTest", mLLVMCtx));
    
    // The IR builder for the llvm compiler
    llvm::IRBuilder<> irBuilder(mLLVMCtx);
    
    // Declare the c standard library function 'printf'
    llvm::FunctionType * printfFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx),
                                                                llvm::ArrayRef<llvm::Type*>
                                                                {
                                                                    llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo(),
                                                                },
                                                                true);
    
    mLLVMMod->getOrInsertFunction("printf", printfFnType);
    
    // Declare the c standard library function 'scanf'
    llvm::FunctionType * scanfFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx),
                                                               llvm::ArrayRef<llvm::Type*>
                                                               {
                                                                   llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo(),
                                                               },
                                                               true);
    
    mLLVMMod->getOrInsertFunction("scanf", scanfFnType);
    
    // Create the function for main
    llvm::FunctionType * mainFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx), {}, false);
    llvm::Function * mainFn = llvm::Function::Create(mainFnType,
                                                     llvm::Function::ExternalLinkage,
                                                     "main",
                                                     mLLVMMod.get());
    
    // Create the block for main and set it as the insert point for ir builder
    llvm::BasicBlock * mainBlock = llvm::BasicBlock::Create(mLLVMCtx, "main_fn_block", mainFn);
    irBuilder.SetInsertPoint(mainBlock);
    
    // Generate the code
    {
        // Do basic forward code generation (most code is generated this way):
        CodegenCtx codegenCtx(mLLVMCtx, irBuilder, *mLLVMMod);
        
        if (!mScope->codegenStmnt(codegenCtx)) {
            return false;
        }
        
        // Do deferred code generation for 'next', 'break' and 'return' statements:
        llvm::BasicBlock * prevInsertBlock = irBuilder.GetInsertBlock();
        
        while (!codegenCtx.deferredCodegenStmnts.empty()) {
            IDeferredCodegenStmnt * stmnt = codegenCtx.deferredCodegenStmnts.back();
            codegenCtx.deferredCodegenStmnts.pop_back();
            
            if (!stmnt->deferredCodegenStmnt(codegenCtx)) {
                return false;
            }
        }
        
        irBuilder.SetInsertPoint(prevInsertBlock);
    }
    
    // Return 0 for success
    irBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(mLLVMCtx), 0));
    return true;
}

bool Module::wasCodeGeneratedOk() {
    return mLLVMMod.get() != nullptr;
}

void Module::dumpIRCodeToStdout() {
    WC_GUARD_ASSERT(mLLVMMod.get());
    mLLVMMod->dump();
}

WC_END_NAMESPACE
