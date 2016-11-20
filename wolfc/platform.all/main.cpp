#include "AST/WCModule.hpp"
#include "Lexer/WCLexer.hpp"
#include "WCFileUtils.hpp"
#include "WCLinearAlloc.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdio>
    #include <llvm/IR/LLVMContext.h>
    #include <memory>
WC_THIRD_PARTY_INCLUDES_END

using namespace wolfc;

int main(int argc, const char * argv[]) {
    // Arg check
    if (argc != 2) {
        std::printf("Usage: %s <Input Wolf File>\n", argv[0]);
        return -1;
    }
    
    // Read the input file
    std::unique_ptr<char[]> inputSrc(wolfc::FileUtils::readTextFileAsCString(argv[1]));
    
    if (!inputSrc) {
        std::printf("Failed to read input Wolf source file '%s'!\n", argv[1]);
        return -1;
    }
    
    // The context for llvm compiler backend
    llvm::LLVMContext llvmContext;
    
    // Run it through the lexer
    Lexer lexer;
    WC_GUARD(lexer.process(inputSrc.get()), -1);
    
    // Linear allocator that we can use
    LinearAlloc linearAlloc(1024 * 1024,        /* 1 MB max alloc */
                            1024 * 1024 * 16,   /* 16 MB block size */
                            128,                /* Space in blocks array to reserve */
                            1024 * 4);          /* Space in sys blocks array to reserve */
    
    {
        // Declare the module
        Module module(llvmContext);
        
        // Parse and codegen the module
        WC_GUARD(module.parseCode(lexer.getTokenList(), linearAlloc), -1);
        WC_GUARD(module.generateCode(), -1);
        
        // Dump the code to stdout!
        module.dumpIRCodeToStdout();
    }
    
    return 0;
}
