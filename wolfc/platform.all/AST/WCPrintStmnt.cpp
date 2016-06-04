#include "WCPrintStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

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
    
    // Parse the inner expression
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr);
    WC_GUARD(assignExpr, nullptr);
    
    // Expect ')' following all that:
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "Expected closing ')' for 'print()' expression!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = tokenPtr;
    ++tokenPtr;
    
    // Create and return the print statement
    return new PrintStmnt(*assignExpr, *printTok, *closingParenTok);
}

PrintStmnt::PrintStmnt(AssignExpr & expr, const Token & startToken, const Token & endToken) :
    mExpr(expr),
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

llvm::Value * PrintStmnt::generateCode(const CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getFunction("printf");
    
    if (!printfFn) {
        compileError("Codegen failed! Can't find 'printf' function!");
        return nullptr;
    }
    
    // Evaluate the code for the argument to printf
    llvm::Value * exprVal = mExpr.generateCode(cgCtx);
    
    if (!exprVal) {
        compileError("Codegen failed! Can't generate code for inner expression value in print statement!");
        return nullptr;
    }
    
    return mExpr.getDataType().genPrintStmntCode(cgCtx, *this, *printfFn, *exprVal);
}

WC_END_NAMESPACE
