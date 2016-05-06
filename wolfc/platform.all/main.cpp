#include "WCMacros.hpp"
#include "WCFileUtils.hpp"
#include <memory>
#include <cstdio>

/*

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

// The context for the llvm compiler
llvm::LLVMContext gLLVMContext;

// The IR builder for the llvm compiler
llvm::IRBuilder<> gIRBuilder(gLLVMContext);

*/

int main(int argc, const char * argv[]) {
    // Arg check
    if (argc != 3) {
        std::printf("Usage: %s <Input Wolf File> <Output LLVM IR File>\n", argv[0]);
        return -1;
    }
    
    // Read the input file
    std::unique_ptr<char32_t[]> inputSrc(WCFileUtils::readUTF8TextFileAsUTF32String(argv[1]));
    
    if (!inputSrc) {
        std::printf("Failed to read input Wolf source file '%s'!\n", argv[1]);
        return -1;
    }
    
    /*
    
    // The module, which holds all the code.
    llvm::Module module("WolfTest", gLLVMContext);
    
    // Declare puts
    llvm::FunctionType * putsFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(gLLVMContext),
                                                              llvm::ArrayRef<llvm::Type*>
                                                              {
                                                                  llvm::Type::getInt8Ty(gLLVMContext)->getPointerTo()
                                                              },
                                                              false);
    
    llvm::Constant * putsFn = module.getOrInsertFunction("puts", putsFnType);
    
    // Create the function for main
    llvm::FunctionType * mainFnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(gLLVMContext), {}, false);
    llvm::Function * mainFn = llvm::Function::Create(mainFnType,
                                                     llvm::Function::ExternalLinkage,
                                                     "main",
                                                     &module);
    
    // Create the block for main and set it as the insert point for ir builder
    llvm::BasicBlock * mainBlock = llvm::BasicBlock::Create(gLLVMContext, "", mainFn);
    gIRBuilder.SetInsertPoint(mainBlock);
    
    // Create string for hello world
    llvm::Value * helloWorldStr = gIRBuilder.CreateGlobalStringPtr("Hello Wolf!");
    gIRBuilder.CreateCall(putsFn, helloWorldStr);
    
    // Return void
    gIRBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(gLLVMContext), 0));
    
    // Dump the code to stdout
    module.dump();
     
    */
     
    return 0;
}
