#include "WCPrintStmnt.hpp"
#include "WCAssert.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStrLit.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PrintStmnt
//-----------------------------------------------------------------------------

bool PrintStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kPrint;
}

PrintStmnt * PrintStmnt::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kPrint) {
        parseError(*tokenPtr, "Expected keyword 'print'!");
        return nullptr;
    }
    
    const Token * printTok = tokenPtr;
    ++tokenPtr;     // Consume 'print'
    
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "Expected '(' following 'print'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume '('
    
    // See what form of the syntax is ahead
    if (StrLit::peek(tokenPtr)) {
        // Print a string literal: parse that
        StrLit * strLit = StrLit::parse(tokenPtr);
        WC_GUARD_ASSERT(strLit, nullptr);
        
        // Expect ')' following that:
        if (tokenPtr->type != TokenType::kRParen) {
            parseError(*tokenPtr, "Expected closing ')' for 'print()' expression!");
            return nullptr;
        }
        
        // Consume closing ')' and return parsed expression
        const Token * closingParenTok = tokenPtr;
        ++tokenPtr;
        return new PrintStmntStrLit(*printTok, *closingParenTok, *strLit);
    }
    else if (BinaryExpr::peek(tokenPtr)) {
        // Print a binary expression: parse that
        BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
        WC_GUARD_ASSERT(binaryExpr, nullptr);
        
        // Expect ')' following all that:
        if (tokenPtr->type != TokenType::kRParen) {
            parseError(*tokenPtr, "Expected closing ')' for 'print()' expression!");
            return nullptr;
        }
        
        // Consume closing ')' and return parsed expression
        const Token * closingParenTok = tokenPtr;
        ++tokenPtr;
        return new PrintStmntBinaryExpr(*printTok, *closingParenTok, *binaryExpr);
    }
    else {
        parseError(*tokenPtr, "Unexpected tokens following 'print' and '('! Expect binary expression or string literal!");
        return nullptr;
    }
    
    WC_RAISE_ASSERTION("This code should not be reached!");
    return nullptr;
}

PrintStmnt::PrintStmnt(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & PrintStmnt::getStartToken() const {
    return mStartToken;
}

const Token & PrintStmnt::getEndToken() const {
    return mEndToken;
}

//-----------------------------------------------------------------------------
// PrintStmntStrLit
//-----------------------------------------------------------------------------

PrintStmntStrLit::PrintStmntStrLit(const Token & startToken, const Token & endToken, StrLit & lit) :
    PrintStmnt(startToken, endToken),
    mLit(lit)
{
    mLit.mParent = this;
}

llvm::Value * PrintStmntStrLit::generateCode(const CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getFunction("printf");
    
    if (!printfFn) {
        compileError("Codegen failed! Can't find 'printf' function!");
        return nullptr;
    }
    
    // Create a format string for printf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "print_stmnt_str_fmt_str");
    
    // Evaluate the code for the argument to printf
    llvm::Value * arg1Val = mLit.generateCode(cgCtx);
    
    if (!arg1Val) {
        compileError("Codegen failed! Can't generate code for string literal!");
        return nullptr;
    }
    
    // Call printf!
    return cgCtx.irBuilder.CreateCall(printfFn, { fmtStr, arg1Val }, "print_stmnt_printf_call");
}

//-----------------------------------------------------------------------------
// PrintStmntBinaryExpr
//-----------------------------------------------------------------------------

PrintStmntBinaryExpr::PrintStmntBinaryExpr(const Token & startToken, const Token & endToken, BinaryExpr & expr) :
    PrintStmnt(startToken, endToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

llvm::Value * PrintStmntBinaryExpr::generateCode(const CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getFunction("printf");
    
    if (!printfFn) {
        compileError("Codegen failed! Can't find 'printf' function!");
        return nullptr;
    }
    
    // Create a format string for printf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%zd", "print_stmnt_int_fmt_str");
    
    // Evaluate the code for the argument to printf
    llvm::Value * arg1Val = mExpr.generateCode(cgCtx);
    
    if (!arg1Val) {
        compileError("Codegen failed! Can't generate code for binary expression!");
        return nullptr;
    }
    
    // Call printf!
    return cgCtx.irBuilder.CreateCall(printfFn, { fmtStr, arg1Val }, "print_stmnt_printf_call");
}

WC_END_NAMESPACE