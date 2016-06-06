#include "WCReadnumExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool ReadnumExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kReadnum;
}

ReadnumExpr * ReadnumExpr::parse(const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kReadnum) {
        parseError(*tokenPtr, "Expected 'readnum' at begining of readnum expression!");
        return nullptr;
    }
    
    const Token * readnumTok = tokenPtr;
    ++tokenPtr;     // Consume 'readnum'
    
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "Expect '(' following 'readnum'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume '('
    
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = tokenPtr;
    ++tokenPtr;     // Consume ')'
    
    return new ReadnumExpr(*readnumTok, *rparenTok);
}

ReadnumExpr::ReadnumExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    
}

const Token & ReadnumExpr::getStartToken() const {
    return mStartToken;
}

const Token & ReadnumExpr::getEndToken() const {
    return mEndToken;
}

bool ReadnumExpr::isLValue() const {
    return false;
}

const DataType & ReadnumExpr::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt);
}

llvm::Value * ReadnumExpr::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * ReadnumExpr::codegenExprEval(const CodegenCtx & cgCtx) {
    // Get scanf
    llvm::Constant * scanfFn = cgCtx.module.getFunction("scanf");
    
    if (!scanfFn) {
        compileError("Codegen failed! Can't find 'scanf' function!");
        return nullptr;
    }
    
    // Create a format string for scanf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%zd", "readnum_expr_fmt_str");
    
    // Create a stack var to hold the output:
    llvm::Value * outputVar = cgCtx.irBuilder.CreateAlloca(cgCtx.irBuilder.getInt64Ty(),
                                                           nullptr,
                                                           "readnum_expr_temp_stack_var");
    
    // Create the call to scanf!
    cgCtx.irBuilder.CreateCall(scanfFn, { fmtStr, outputVar }, "readnum_expr_scanf_call");
    
    // And return the stack var
    return cgCtx.irBuilder.CreateLoad(outputVar, "readnum_expr_load_temp_stack_var");
}

WC_END_NAMESPACE
