//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AST/Nodes/Module.hpp"
#include "AST/ParseCtx.hpp"
#include "Backend/LLVM/Codegen/Codegen.hpp"
#include "Backend/LLVM/CodegenCtx.hpp"
#include "FileUtils.hpp"
#include "Lexer/Lexer.hpp"
#include "LinearAlloc.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdio>
    #include <memory>

    #ifdef _MSC_VER
        #include <Windows.h>
    #endif
WC_THIRD_PARTY_INCLUDES_END

/**
 * Temporary name for the wolfc module.
 * TODO: Some day this will go away and be based on the filename or whatever.
 */
static const std::string kModuleName = "WolfTest";

/* Compiles the given AST */
static bool compileAST(const Wolfc::AST::Module * astModule, const char * fromSrcFile) {
    // Create a codegen context and do the code generation using the LLVM backend
    Wolfc::LLVMBackend::CodegenCtx codegenCtx(kModuleName);
    Wolfc::LLVMBackend::Codegen codegen(codegenCtx);
    codegen.visit(*astModule);
    
    // Emit compile warnings to stdout if there are any
    if (codegenCtx.hasWarnings()) {
        std::fprintf(stdout, "Compile warnings emitted for source file '%s'! Warning messages follow:\n", fromSrcFile);
        
        for (const std::string & warningMsg : codegenCtx.getWarningMsgs()) {
            std::fprintf(stdout, "%s\n", warningMsg.c_str());
        }
    }
    
    // Emit compile errors to stderr if there are any and fail
    if (!codegenCtx.mLLVMModule.get() || codegenCtx.hasErrors()) {
        std::fprintf(stderr, "Compile failed for source file '%s'! Error messages follow:\n", fromSrcFile);
        
        for (const std::string & errorMsg : codegenCtx.getErrorMsgs()) {
            std::fprintf(stderr, "%s\n", errorMsg.c_str());
        }
        
        return false;
    }
    
    // Dump the IR code for the module to stdout
    if (!codegenCtx.dumpIRCodeToStdout()) {
        return false;
    }
    
    // Success!
    return true;
}

/* Compiles the given list of tokens */
static bool compileTokens(const Wolfc::Token * tokenList, const char * fromSrcFile) {
    // Linear allocator that we can use
    Wolfc::LinearAlloc linearAlloc(
        1024 * 1024,        /* 1 MB max alloc */
        1024 * 1024 * 16,   /* 16 MB block size */
        128,                /* Space in blocks array to reserve */
        1024 * 4            /* Space in sys blocks array to reserve */
    );

    // Declare and parse the module AST
    Wolfc::AST::ParseCtx parseCtx(tokenList, linearAlloc);
    Wolfc::AST::Module * astModule = Wolfc::AST::Module::parse(parseCtx);
    
    // Emit parse warnings to stdout if there are any
    if (parseCtx.hasWarnings()) {
        std::fprintf(stdout, "Parse warnings emitted for source file '%s'! Warning messages follow:\n", fromSrcFile);
        
        for (const std::string & warningMsg : parseCtx.getWarningMsgs()) {
            std::fprintf(stdout, "%s\n", warningMsg.c_str());
        }
    }
    
    // Emit parse errors to stderr if there are any and fail
    if (!astModule || parseCtx.hasErrors()) {
        std::fprintf(stderr, "Parsing failed for source file '%s'! Error messages follow:\n", fromSrcFile);
        
        for (const std::string & errorMsg : parseCtx.getErrorMsgs()) {
            std::fprintf(stderr, "%s\n", errorMsg.c_str());
        }
        
        return false;
    }
    
    // Compile the AST
    return compileAST(astModule, fromSrcFile);
}

/* Compiles the given source text */
static bool compileSourceText(const char * srcText, const char * fromSrcFile) {
    // Split the text up into language tokens with the lexer
    Wolfc::Lexer lexer;
    
    if (!lexer.process(srcText)) {
        return false;
    }
    
    // Compile the list of tokens generated
    return compileTokens(lexer.getTokenList(), fromSrcFile);
}

/* Compiles the given source file */
static bool compileSourceFile(const char * srcFilePath) {
    // Read the source file
    std::unique_ptr<char[]> inputSrc(Wolfc::FileUtils::readTextFileAsCString(srcFilePath));
    
    if (!inputSrc) {
        std::printf("Failed to read input Wolf source file '%s'!\n", srcFilePath);
        return false;
    }
    
    // Compile the source text
    return compileSourceText(inputSrc.get(), srcFilePath);
}

/* The actual implementation of main */
static int mainImpl(int argc, const char * argv[]) {
    // Arg check
    if (argc != 2) {
        std::printf("Usage: %s <Input Wolf File>\n", argv[0]);        
        return -1;
    }

    // Begin the compiler pipeline
    if (!compileSourceFile(argv[1])) {
        return -1;
    }

    // Everything went well!
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
