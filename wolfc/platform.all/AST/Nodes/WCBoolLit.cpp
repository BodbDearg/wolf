#include "WCBoolLit.hpp"

#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

#warning FIXME - Codegen
#if 0
WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Constants.h>
WC_THIRD_PARTY_INCLUDES_END
#endif

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool BoolLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kTrue || tokenPtr->type == TokenType::kFalse;
}

BoolLit * BoolLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.curTok)) {
        parseCtx.error("Expected 'bool' literal!");
        return nullptr;
    }
    
    BoolLit * boolLit = WC_NEW_AST_NODE(parseCtx, BoolLit, *parseCtx.curTok);
    parseCtx.nextTok();
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
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

#warning FIXME - Codegen
#if 0
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
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
