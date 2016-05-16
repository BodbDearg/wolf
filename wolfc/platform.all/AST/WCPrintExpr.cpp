#include "WCPrintExpr.hpp"
#include "WCAssert.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStrLit.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool PrintExpr::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kPrint;
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
        StrLit * strLit = StrLit::parse(tokenPtr);
        WC_GUARD_ASSERT(strLit, nullptr);
        parsedPrintExpr = new PrintExprStrLit(*strLit);
    }
    else if (BinaryExpr::peek(tokenPtr)) {
        BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
        WC_GUARD_ASSERT(binaryExpr, nullptr);
        parsedPrintExpr = new PrintExprBinaryExpr(*binaryExpr);
    }
    else {
        error(*tokenPtr, "Unexpected tokens following 'print' and '('! Expect binary expression or string literal!");
        return nullptr;
    }
    
    // Must have parsed ok!
    WC_GUARD(parsedPrintExpr, nullptr);
    
    // Expect ')' following all that:
    if (tokenPtr->type != TokenType::kRParen) {
        error(*tokenPtr, "Expected closing ')' for 'print()' expression!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    ++tokenPtr;
    return parsedPrintExpr;
}

PrintExprStrLit::PrintExprStrLit(StrLit & lit) : mLit(lit) {
    mLit.mParent = this;
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

PrintExprBinaryExpr::PrintExprBinaryExpr(BinaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
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
