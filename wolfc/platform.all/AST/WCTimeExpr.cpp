#include "WCTimeExpr.hpp"

#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool TimeExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTime;
}

TimeExpr * TimeExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (tokenPtr->type != TokenType::kTime) {
        parseError(*tokenPtr, "Expected 'time' at begining of time() expression!");
        return nullptr;
    }
    
    const Token * readnumTok = tokenPtr;
    ++tokenPtr;     // Consume 'time'
    
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "Expect '(' following 'time'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume '('
    
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "Expect ')' following '('!");
        return nullptr;
    }
    
    const Token * rparenTok = tokenPtr;
    ++tokenPtr;     // Consume ')'
    
    return WC_NEW_AST_NODE(alloc, TimeExpr, *readnumTok, *rparenTok);
}

TimeExpr::TimeExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    
}

const Token & TimeExpr::getStartToken() const {
    return mStartToken;
}

const Token & TimeExpr::getEndToken() const {
    return mEndToken;
}

bool TimeExpr::isLValue() {
    return false;
}

bool TimeExpr::isConstExpr() {
    return false;
}

DataType & TimeExpr::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt);
}

llvm::Value * TimeExpr::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a 'time()' expression result!");
    return nullptr;
}

llvm::Value * TimeExpr::codegenExprEval(CodegenCtx & cgCtx) {
    // Get 'time' C function
    llvm::Constant * timeFN = cgCtx.module.getLLVMModuleRef().getFunction("time");
    
    if (!timeFN) {
        compileError("Codegen failed! Can't find 'time' function!");
        return nullptr;
    }

    // This is the value we will pass to the time() function, a null pointer
    llvm::Type * int64PtrTy = cgCtx.irBuilder.getInt64Ty()->getPointerTo();
    WC_ASSERT(int64PtrTy);
    llvm::Constant * nullInt64Ptr = llvm::ConstantPointerNull::getNullValue(int64PtrTy);
    WC_ASSERT(nullInt64Ptr);
    
    // Create the call to time!
    llvm::Value * callInst = cgCtx.irBuilder.CreateCall(timeFN, { nullInt64Ptr }, "TimeExpr:result");
    WC_ASSERT(callInst);
    return callInst;
}

llvm::Constant * TimeExpr::codegenExprConstEval(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cannot evaluate a 'time()' expression at compile time!");
    return nullptr;
}

WC_END_NAMESPACE
