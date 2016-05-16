#include "WCIntLit.hpp"
#include "WCToken.hpp"
#include "WCCodegenCtx.hpp"

WC_BEGIN_NAMESPACE

bool IntLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIntLit;
}

IntLit * IntLit::parse(ASTNode & parent, const Token *& tokenPtr) {
    if (tokenPtr->type != TokenType::kIntLit) {
        error(*tokenPtr, "Expected integer literal!");
        return nullptr;
    }
    
    IntLit * intLit = new IntLit(parent, *tokenPtr);
    ++tokenPtr;
    return intLit;
}

IntLit::IntLit(ASTNode & parent, const Token & token) :
    ASTNodeCodegen(parent),
    mToken(token)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * IntLit::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.getInt64(mToken.data.intVal);
}

WC_END_NAMESPACE
