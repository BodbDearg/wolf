#include "AST/Nodes/WCModule.hpp"
#include "Codegen/LLVM/Module.hpp"
#include "Lexer/WCLexer.hpp"
#include "WCFileUtils.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdio>
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

    // Declare and parse the module AST
    Wolfc::AST::Module astModule;
    Wolfc::AST::ParseCtx parseCtx(lexer.getTokenList(), linearAlloc);
    
    // Emit parse warnings to stdout if there are any
    if (parseCtx.hasWarnings()) {
        std::printf("Warnings emitted for source file '%s'! Warning messages follow:\n", argv[1]);
        
        for (const std::string & warningMsg : parseCtx.warningMsgs) {
            std::fprintf(stdout, "%s\n", warningMsg.c_str());
        }
    }

    // Emit parse errors to stderr if there are any and fail
    if (!astModule.parse(parseCtx) || parseCtx.hasErrors()) {
        std::printf("Parsing failed for source file '%s'! Error messages follow:\n", argv[1]);
        
        for (const std::string & errorMsg : parseCtx.errorMsgs) {
            std::fprintf(stderr, "%s\n", errorMsg.c_str());
        }
        
        return -1;
    }

#warning FIXME - Codegen
#if 0
    // Codegen the module
    if (!module.generateCode()) {
        std::printf("Compile failed for source file '%s'!\n", argv[1]);
        return -1;
    }

    // Dump the code to stdout!
    module.dumpIRCodeToStdout();
#endif

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
