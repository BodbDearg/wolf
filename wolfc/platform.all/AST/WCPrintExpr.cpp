#include "WCPrintExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStrLit.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool PrintExpr::peek(const Token * tokenPtr) {
    if (tokenPtr[0].type == TokenType::kPrint) {
        return true;
    }
    
    return false;
}

PrintExpr * PrintExpr::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kPrint) {
        error(*tokenPtr, "Expected keyword 'print'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume 'print'
    
    if (tokenPtr->type != TokenType::kLParen) {
        error(*tokenPtr, "Expected '(' following 'print'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume '('
    
    // See what form of the syntax is ahead
    PrintExpr * parsedPrintExpr = nullptr;
    
    if (StrLit::peek(tokenPtr)) {
        parsedPrintExpr = PrintExprStrLit::parse(tokenPtr);
    }
    else if (BinaryExpr::peek(tokenPtr)) {
        parsedPrintExpr = PrintExprBinaryExpr::parse(tokenPtr);
    }
    else {
        error(*tokenPtr, "Unexpected tokens following 'print' and '('! Expect binary expression or string literal!");
        return nullptr;
    }
    
    // Must have parsed ok!
    if (!parsedPrintExpr) {
        return nullptr;
    }
    
    // Expect ')' following all that:
    if (tokenPtr->type != TokenType::kRParen) {
        error(*tokenPtr, "Expected closing ')' for 'print()' expression!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    ++tokenPtr;
    return parsedPrintExpr;
}

PrintExprStrLit * PrintExprStrLit::parse(const Token *& tokenPtr) {
    StrLit * strLit = StrLit::parse(tokenPtr);
    
    if (!strLit) {
        return nullptr;
    }
    
    return new PrintExprStrLit(*strLit);
}

PrintExprStrLit::PrintExprStrLit(StrLit & lit) : mLit(lit) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * PrintExprStrLit::generateCode(const CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getFunction("printf");
    
    if (!printfFn) {
        error("Codegen failed! Can't find 'printf' function!");
        return nullptr;
    }
    
    // Create a format string for printf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s");
    
    // Evaluate the code for the argument to printf
    llvm::Value * arg1Val = mLit.generateCode(cgCtx);
    
    if (!arg1Val) {
        error("Codegen failed! Can't generate code for string literal!");
        return nullptr;
    }
    
    // Call printf!
    return cgCtx.irBuilder.CreateCall(printfFn, { fmtStr, arg1Val });
}

PrintExprBinaryExpr * PrintExprBinaryExpr::parse(const Token *& tokenPtr) {
    BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
    
    if (!binaryExpr) {
        return nullptr;
    }
    
    return new PrintExprBinaryExpr(*binaryExpr);
}

PrintExprBinaryExpr::PrintExprBinaryExpr(BinaryExpr & expr) : mExpr(expr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * PrintExprBinaryExpr::generateCode(const CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getFunction("printf");
    
    if (!printfFn) {
        error("Codegen failed! Can't find 'printf' function!");
        return nullptr;
    }
    
    // Create a format string for printf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%zu");
    
    // Evaluate the code for the argument to printf
    llvm::Value * arg1Val = mExpr.generateCode(cgCtx);
    
    if (!arg1Val) {
        error("Codegen failed! Can't generate code for binary expression!");
        return nullptr;
    }
    
    // Call printf!
    return cgCtx.irBuilder.CreateCall(printfFn, { fmtStr, arg1Val });
}

WC_END_NAMESPACE
