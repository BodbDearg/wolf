#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <iostream>

// The context for the llvm compiler
llvm::LLVMContext gLLVMContext;

// The IR builder for the llvm compiler
llvm::IRBuilder<> gIRBuilder(gLLVMContext);

int main(int argc, const char * argv[]) {
    // The module, which holds all the code.
    llvm::Module module("TestModule", gLLVMContext);
    
    // Dump the code to stdout
    module.dump();
    return 0;
}
