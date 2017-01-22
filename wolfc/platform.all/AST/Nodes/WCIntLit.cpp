#include "WCIntLit.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

// TODO: Make this be for ALL numeric literals

bool IntLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIntLit;
}

IntLit * IntLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected integer literal!");
        return nullptr;
    }
    
    IntLit * intLit = WC_NEW_AST_NODE(parseCtx, IntLit, *parseCtx.tok());
    parseCtx.nextTok();
    return intLit;
}

IntLit::IntLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void IntLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * IntLit::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: update message here when we convert to a more generic numeric literal
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an 'int' literal!");
    return nullptr;
}

llvm::Constant * IntLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: data type should be based on the numeric literal and precision suffixes used
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), mToken.data.intVal);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
