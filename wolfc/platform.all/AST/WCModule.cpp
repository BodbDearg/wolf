#include "WCModule.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDeclDefs.hpp"
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
    mLLVMMod.reset();
    
    if (!mDeclDefs) {
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
    
    // Create the codegen context and generate code for the entire module
    CodegenCtx codegenCtx(mLLVMCtx, irBuilder, *mLLVMMod);
    return mDeclDefs->codegen(codegenCtx);
}

bool Module::wasCodeGeneratedOk() {
    // FIXME: this can be not null and still not be generated ok
    return mLLVMMod.get() != nullptr;
}

void Module::dumpIRCodeToStdout() {
    WC_GUARD_ASSERT(mLLVMMod.get());
    mLLVMMod->dump();
}

WC_END_NAMESPACE
