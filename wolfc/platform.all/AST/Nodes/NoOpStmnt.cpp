#include "NoOpStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool NoOpStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNoOp;
}

NoOpStmnt * NoOpStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'noop'
    if (parseCtx.tok()->type != TokenType::kNoOp) {
        parseCtx.error("'noop' statement expected!");
        return nullptr;
    }
    
    // Consume and save the token and return the parsed statement
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    return WC_NEW_AST_NODE(parseCtx, NoOpStmnt, *startToken);
}

NoOpStmnt::NoOpStmnt(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void NoOpStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NoOpStmnt::getStartToken() const {
    return mToken;
}

const Token & NoOpStmnt::getEndToken() const {
    return mToken;
}

bool NoOpStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
