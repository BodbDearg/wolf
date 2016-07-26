#include "WCStrLit.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool StrLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kStrLit;
}

StrLit * StrLit::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected string literal!");
        return nullptr;
    }
    
    StrLit * intLit = WC_NEW_AST_NODE(alloc, StrLit, *tokenPtr);
    ++tokenPtr;
    return intLit;
}

StrLit::StrLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & StrLit::getStartToken() const {
    return mToken;
}

const Token & StrLit::getEndToken() const {
    return mToken;
}

bool StrLit::isLValue() const {
    return false;
}

const DataType & StrLit::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kStr);
}

llvm::Value * StrLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * StrLit::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: need some sort of LUT for the module so we only have unique string instances
    return cgCtx.irBuilder.CreateGlobalStringPtr(mToken.data.strVal.ptr, "usr_string_lit");
}

llvm::Constant * StrLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    #warning TODO: implement constant evaluation
    WC_UNUSED_PARAM(cgCtx);
    compileError("Constant evaluation supported yet for this tyoe of expression!");
    return nullptr;
}

WC_END_NAMESPACE
