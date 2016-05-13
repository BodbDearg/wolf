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
    std::unique_ptr<char32_t[]> inputSrc(wolfc::FileUtils::readUTF8TextFileAsUTF32String(argv[1]));
    
    if (!inputSrc) {
        std::printf("Failed to read input Wolf source file '%s'!\n", argv[1]);
        return -1;
    }
    
    // The context for llvm compiler backend
    llvm::LLVMContext llvmContext;
    
    // Run it through the lexer
    wolfc::Lexer lexer;
    
    if (!lexer.process(inputSrc.get())) {
        return -1;
    }
    
    // Generate the module and codegen
    wolfc::Module module(llvmContext);
    
    if (!module.parseCode(lexer.getTokenList())) {
        return -1;
    }
    
    if (!module.generateCode()) {
        return -1;
    }
    
    // Dump the code to stdout!
    module.dumpIRCodeToStdout();
    return 0;
}
