#include "WCIntLit.hpp"

#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool IntLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIntLit;
}

IntLit * IntLit::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected integer literal!");
        return nullptr;
    }
    
    IntLit * intLit = WC_NEW_AST_NODE(alloc, IntLit, *tokenPtr);
    ++tokenPtr;
    return intLit;
}

IntLit::IntLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & IntLit::getStartToken() const {
    return mToken;
}

const Token & IntLit::getEndToken() const {
    return mToken;
}

bool IntLit::isLValue() {
    return false;
}

bool IntLit::isConstExpr() {
    return true;
}

DataType & IntLit::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt);
}

llvm::Value * IntLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an 'int' literal!");
    return nullptr;
}

llvm::Value * IntLit::codegenExprEval(CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt64(mToken.data.intVal);
}

llvm::Constant * IntLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), mToken.data.intVal);
}

WC_END_NAMESPACE
