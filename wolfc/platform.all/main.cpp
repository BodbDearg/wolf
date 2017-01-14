#include "AST/WCModule.hpp"
#include "Lexer/WCLexer.hpp"
#include "WCFileUtils.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdio>
    #include <llvm/IR/LLVMContext.h>
    #include <memory>

    #ifdef _MSC_VER
        #include <Windows.h>
    #endif
WC_THIRD_PARTY_INCLUDES_END

/* The actual implementation of main */
static int mainImpl(int argc, const char * argv[]) {
    // Arg check
    if (argc != 2) {
        std::printf("Usage: %s <Input Wolf File>\n", argv[0]);        
        return -1;
    }

    // Read the input file
    std::unique_ptr<char[]> inputSrc(Wolfc::FileUtils::readTextFileAsCString(argv[1]));

    if (!inputSrc) {
        std::printf("Failed to read input Wolf source file '%s'!\n", argv[1]);
        return -1;
    }

    // The context for llvm compiler backend
    llvm::LLVMContext llvmContext;

    // Run it through the lexer
    Wolfc::Lexer lexer;
    WC_GUARD(lexer.process(inputSrc.get()), -1);

    // Linear allocator that we can use
    Wolfc::LinearAlloc linearAlloc(
        1024 * 1024,        /* 1 MB max alloc */
        1024 * 1024 * 16,   /* 16 MB block size */
        128,                /* Space in blocks array to reserve */
        1024 * 4            /* Space in sys blocks array to reserve */
    );

    {
        // Declare the module
        Wolfc::Module module(llvmContext);
        
        // Create a parsing context and parse the module
        Wolfc::ParseCtx parseCtx(lexer.getTokenList(), linearAlloc);

        if (!module.parse(parseCtx)) {
            std::printf("Parsing failed for source file '%s'!\n", argv[1]);
            return -1;
        }

        // Codegen the module
        if (!module.generateCode()) {
            std::printf("Compile failed for source file '%s'!\n", argv[1]);
            return -1;
        }

        // Dump the code to stdout!
        module.dumpIRCodeToStdout();
    }

    return 0;
}

/* In windows builds, pauses for the debugger. Does nothing in other builds. */
static void msvcPauseForDebugger() {
#ifdef _MSC_VER
    if (IsDebuggerPresent()) {
        printf("Press enter to continue...");
        getchar();
    }
#endif
}

/* Program entry point */
int main(int argc, const char * argv[]) {
    int statusCode = -1;

    try {
        statusCode = mainImpl(argc, argv);
    }
    catch (...) {
        printf("An uncaught exception occurred! Unsure what the problem was.");
    }

    msvcPauseForDebugger();
    return statusCode;
}
