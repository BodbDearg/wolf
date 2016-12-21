#include "WCIntLit.hpp"

#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

// TODO: Make this be for ALL numeric literals

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
    // TODO: data type should be based on the numeric literal and precision suffixes used
    return PrimitiveDataTypes::getDefaultUIntType();
}

llvm::Value * IntLit::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: update message here when we convert to a more generic numeric literal
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an 'int' literal!");
    return nullptr;
}

llvm::Value * IntLit::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: data type should be based on the numeric literal and precision suffixes used
    return cgCtx.irBuilder.getInt64(mToken.data.intVal);
}

llvm::Constant * IntLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: data type should be based on the numeric literal and precision suffixes used
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), mToken.data.intVal);
}

WC_END_NAMESPACE
