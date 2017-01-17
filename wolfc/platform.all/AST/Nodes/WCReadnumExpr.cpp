#include "WCReadnumExpr.hpp"

#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool ReadnumExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kReadnum;
}

ReadnumExpr * ReadnumExpr::parse(ParseCtx & parseCtx) {
    if (parseCtx.tok()->type != TokenType::kReadnum) {
        parseCtx.error("Expected 'readnum' at begining of readnum() expression!");
        return nullptr;
    }
    
    const Token * readnumTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume 'readnum'
    
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expect '(' following 'readnum'!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Consume '('
    
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume ')'
    
    return WC_NEW_AST_NODE(parseCtx, ReadnumExpr, *readnumTok, *rparenTok);
}

ReadnumExpr::ReadnumExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & ReadnumExpr::getStartToken() const {
    return mStartToken;
}

const Token & ReadnumExpr::getEndToken() const {
    return mEndToken;
}

bool ReadnumExpr::isLValue() {
    return false;
}

bool ReadnumExpr::isConstExpr() {
    return false;
}

DataType & ReadnumExpr::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64);
}

#warning FIXME - Codegen
#if 0
llvm::Value * ReadnumExpr::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a 'readnum()' expression result!");
    return nullptr;
}

llvm::Value * ReadnumExpr::codegenExprEval(CodegenCtx & cgCtx) {
    // Get 'scanf' C function
    llvm::Constant * scanfFn = cgCtx.module.getLLVMModuleRef().getFunction("scanf");
    
    if (!scanfFn) {
        compileError("Codegen failed! Can't find 'scanf' function!");
        return nullptr;
    }
    
    // Get 'getchar' C function
    llvm::Constant * getcharFn = cgCtx.module.getLLVMModuleRef().getFunction("getchar");
    
    if (!getcharFn) {
        compileError("Codegen failed! Can't find 'getchar' function!");
        return nullptr;
    }
    
    // Create a format string for scanf
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%zd", "ReadnumExpr:fmt_str");
    WC_ASSERT(fmtStr);
    
    // Create a stack var to hold the output:
    llvm::Value * outputVar = cgCtx.irBuilder.CreateAlloca(cgCtx.irBuilder.getInt64Ty(),
                                                           nullptr,
                                                           "ReadnumExpr:tmp_stack_var");
    
    WC_ASSERT(outputVar);
    
    // Create the call to scanf!
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateCall(scanfFn,
                                              { fmtStr, outputVar },
                                              "ReadnumExpr:scanf_call"));
    
    // Consume the following return character
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateCall(getcharFn,
                                              {},
                                              "ReadnumExpr:getchar_call"));
    
    // And return the stack var
    llvm::Value * returnValue = cgCtx.irBuilder.CreateLoad(outputVar, "ReadnumExpr:load_tmp_stack_var");
    WC_ASSERT(returnValue);
    return returnValue;
}

llvm::Constant * ReadnumExpr::codegenExprConstEval(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cannot evaluate a 'readnum()' expression at compile time!");
    return nullptr;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
