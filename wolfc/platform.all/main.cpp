#include "WCFileUtils.hpp"
#include "WCLexer.hpp"
#include "WCModule.hpp"
#include <memory>
#include <cstdio>

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/LLVMContext.h>
WC_THIRD_PARTY_INCLUDES_END

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
    wolfc::Lexer lexer;
    WC_GUARD(lexer.process(inputSrc.get()), -1);
    
    // Generate the module and codegen
    wolfc::Module module(llvmContext);
    WC_GUARD(module.parseCode(lexer.getTokenList()), -1);
    WC_GUARD(module.generateCode(), -1);
    
    // Dump the code to stdout!
    module.dumpIRCodeToStdout();
    return 0;
}
