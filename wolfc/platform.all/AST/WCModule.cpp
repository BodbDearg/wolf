#include "WCModule.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDeclDefs.hpp"
#include "WCFunc.hpp"
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
    WC_ASSERT(mDeclDefs);
    return mDeclDefs->getStartToken();
}

const Token & Module::getEndToken() const {
    WC_ASSERT(mDeclDefs);
    return mDeclDefs->getEndToken();
}

bool Module::parseCode(const Token * tokenList, LinearAlloc & alloc) {
    mDeclDefs = DeclDefs::parse(tokenList, alloc);
    WC_GUARD(mDeclDefs, false);
    mDeclDefs->mParent = this;
    
    if (tokenList->type != TokenType::kEOF) {
        parseError(*tokenList, "Expected EOF at end of module code!");
        mDeclDefs = nullptr;
        return false;
    }
    
    return true;
}

bool Module::generateCode() {
    // Clear out previous code and check we parsed ok
    mLLVMModule.reset();
    
    if (!mDeclDefs) {
        compileError("Can't generate code, parsing was not successful!");
        return false;
    }
    
    // TODO: alloc this memory with the linear allocator
    // Create module
    mLLVMModule.reset(new llvm::Module("WolfTest", mLLVMCtx));
    
    // The IR builder for the llvm compiler
    llvm::IRBuilder<> irBuilder(mLLVMCtx);
    
    // Declare the c standard library function 'printf'
    llvm::FunctionType * printfFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx),
                                                                llvm::ArrayRef<llvm::Type*>
                                                                {
                                                                    llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo(),
                                                                },
                                                                true);
    
    mLLVMModule->getOrInsertFunction("printf", printfFnType);
    
    // Declare the c standard library function 'scanf'
    llvm::FunctionType * scanfFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx),
                                                               llvm::ArrayRef<llvm::Type*>
                                                               {
                                                                   llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo(),
                                                               },
                                                               true);
    
    mLLVMModule->getOrInsertFunction("scanf", scanfFnType);
    
    // Create the codegen context
    CodegenCtx codegenCtx(mLLVMCtx, irBuilder, *this);
    
    // Do the immediate forward code generation
    WC_GUARD(mDeclDefs->codegen(codegenCtx), false);
    
    // Now do any deferred code generation that needs to be done.
    // There may be multiple passes here for this loop:
    while (!codegenCtx.deferredCodegenCallbacks.empty()) {
        // Make a copy of the list because deferred codegen can itself cause more deferred codegen.
        // Also clear the list so the callbacks can start generating their own deferred logic:
        auto deferredCodegenCallbacks = codegenCtx.deferredCodegenCallbacks;
        codegenCtx.deferredCodegenCallbacks.clear();
        
        // Do deferred codegen:
        for (const auto & callback : deferredCodegenCallbacks) {
            WC_GUARD(callback(codegenCtx), false);
        }
    }

    return true;    // Suceeded!
}

bool Module::wasCodeGeneratedOk() {
    // FIXME: this can be not null and still not be generated ok
    return mLLVMModule.get() != nullptr;
}

void Module::dumpIRCodeToStdout() {
    WC_GUARD_ASSERT(mLLVMModule.get());
    mLLVMModule->dump();
}

bool Module::registerFunc(Func & func) {
    std::string funcName = func.name();
    
    if (getFunc(funcName)) {
        return false;
    }
    
    mFuncs[funcName] = &func;
    return true;
}

Func * Module::getFunc(const std::string & name) const {
    auto iter = mFuncs.find(name);
    WC_GUARD(iter != mFuncs.end(), nullptr);
    return iter->second;
}

WC_END_NAMESPACE
