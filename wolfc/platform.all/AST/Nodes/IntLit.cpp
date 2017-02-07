#include "IntLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

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

const DataType & IntLit::dataType() const {
    // TODO: data type should be based on the numeric literal and precision suffixes used
    return PrimitiveDataTypes::getDefaultUIntType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
