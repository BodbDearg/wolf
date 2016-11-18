#include "WCBoolLit.hpp"

#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool BoolLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTrue || tokenPtr->type == TokenType::kFalse;
}

BoolLit * BoolLit::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected 'bool' literal!");
        return nullptr;
    }
    
    BoolLit * boolLit = WC_NEW_AST_NODE(alloc, BoolLit, *tokenPtr);
    ++tokenPtr;
    return boolLit;
}

BoolLit::BoolLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & BoolLit::getStartToken() const {
    return mToken;
}

const Token & BoolLit::getEndToken() const {
    return mToken;
}

bool BoolLit::isLValue() {
    return false;
}

bool BoolLit::isConstExpr() {
    return true;
}

DataType & BoolLit::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * BoolLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of 'bool' literal!");
    return nullptr;
}

llvm::Value * BoolLit::codegenExprEval(CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt1(mToken.type == TokenType::kTrue);
}

llvm::Constant * BoolLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(cgCtx.llvmCtx),
                                  mToken.type == TokenType::kTrue ? 1 : 0);
}

WC_END_NAMESPACE
