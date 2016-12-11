#include "WCPrintStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool PrintStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kPrint;
}

PrintStmnt * PrintStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (tokenPtr->type != TokenType::kPrint) {
        parseError(*tokenPtr, "Expected keyword 'print' for 'print()' statement!");
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
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(assignExpr, nullptr);
    
    // Expect ')' following all that:
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "Expected closing ')' for 'print()' statement!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = tokenPtr;
    ++tokenPtr;
    
    // Create and return the print statement
    return WC_NEW_AST_NODE(alloc, PrintStmnt, *printTok, *assignExpr, *closingParenTok);
}

PrintStmnt::PrintStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

const Token & PrintStmnt::getStartToken() const {
    return mStartToken;
}

const Token & PrintStmnt::getEndToken() const {
    return mEndToken;
}

bool PrintStmnt::codegen(CodegenCtx & cgCtx) {
    // Get printf
    llvm::Constant * printfFn = cgCtx.module.getLLVMModuleRef().getFunction("printf");
    
    if (!printfFn) {
        compileError("Codegen failed! Can't find 'printf' function!");
        return false;
    }
    
    // Evaluate the code for the argument to printf
    llvm::Value * exprVal = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(exprVal, false);
    return mExpr.dataType().codegenPrintStmnt(cgCtx, *this, *printfFn, *exprVal);
}

WC_END_NAMESPACE
