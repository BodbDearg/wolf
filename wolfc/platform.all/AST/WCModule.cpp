#include "WCModule.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

Module::Module(llvm::LLVMContext & llvmCtx) : mLLVMCtx(llvmCtx) {
    WC_EMPTY_FUNC_BODY()
}

Module::~Module() {
    // Defined here so callee doesn't need to know details of objects destroyed by std::unique_ptr...
    WC_EMPTY_FUNC_BODY()
}

bool Module::parseCode(const Token * tokenList) {
    mExpr.reset(BinaryExpr::parse(tokenList));
    
    if (!mExpr) {
        return false;
    }
    
    if (tokenList->type != TokenType::kEOF) {
        error(*tokenList, "Expected EOF following binary expression!");
        mExpr.reset();
        return false;
    }
    
    return true;
}

bool Module::generateCode() {
    // Clear out previous code and check we parsed ok
    mLLVMMod.reset();
    
    if (!mExpr) {
        error("Can't generate code, parsing was not successful!");
        return false;
    }
    
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
    
    llvm::Constant * printfFn = mLLVMMod->getOrInsertFunction("printf", printfFnType);
    
    // Create the function for main
    llvm::FunctionType * mainFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(mLLVMCtx), {}, false);
    llvm::Function * mainFn = llvm::Function::Create(mainFnType,
                                                     llvm::Function::ExternalLinkage,
                                                     "main",
                                                     mLLVMMod.get());
    
    // Create the block for main and set it as the insert point for ir builder
    llvm::BasicBlock * mainBlock = llvm::BasicBlock::Create(mLLVMCtx, "", mainFn);
    irBuilder.SetInsertPoint(mainBlock);
    
    // Create the string to print the program result
    llvm::Value * helloWorldStr = irBuilder.CreateGlobalStringPtr("Program result is: %zu\n");
    
    // Evaluate the expression result
    llvm::Value * exprResult = mExpr->generateCode(CodegenCtx(irBuilder, *mLLVMMod));
    
    if (!exprResult) {
        mLLVMMod.reset();
        error("Codegen failed! Failed to generate code for the module!");
    }
    
    // Call printf!
    irBuilder.CreateCall(printfFn, { helloWorldStr, exprResult });
    
    // Return 0 for success
    irBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(mLLVMCtx), 0));
    return true;
}

bool Module::wasCodeGeneratedOk() {
    return mLLVMMod.get() != nullptr;
}

void Module::dumpIRCodeToStdout() {
    WC_ASSERT(mLLVMMod.get());
    
    if (!mLLVMMod.get()) {
        return;
    }
    
    mLLVMMod->dump();
}

WC_END_NAMESPACE
