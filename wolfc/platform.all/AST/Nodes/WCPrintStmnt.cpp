#include "WCPrintStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCParseCtx.hpp"

#warning FIXME - Codegen
#if 0
WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END
#endif

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool PrintStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kPrint;
}

PrintStmnt * PrintStmnt::parse(ParseCtx & parseCtx) {
    if (parseCtx.curTok->type != TokenType::kPrint) {
        parseError(parseCtx, "Expected keyword 'print' for 'print()' statement!");
        return nullptr;
    }
    
    const Token * printTok = parseCtx.curTok;
    parseCtx.nextTok();     // Consume 'print'
    
    if (parseCtx.curTok->type != TokenType::kLParen) {
        parseError(parseCtx, "Expected '(' following 'print'!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Consume '('
    
    // Parse the inner expression
    AssignExpr * assignExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(assignExpr, nullptr);
    
    // Expect ')' following all that:
    if (parseCtx.curTok->type != TokenType::kRParen) {
        parseError(parseCtx, "Expected closing ')' for 'print()' statement!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = parseCtx.curTok;
    parseCtx.nextTok();
    
    // Create and return the print statement
    return WC_NEW_AST_NODE(parseCtx, PrintStmnt, *printTok, *assignExpr, *closingParenTok);
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

#warning FIXME - Codegen
#if 0
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
#endif

bool PrintStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
