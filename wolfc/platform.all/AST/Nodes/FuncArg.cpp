//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "FuncArg.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Identifier.hpp"
#include "LinearAlloc.hpp"
#include "Type.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool FuncArg::peek(const Token * currentToken) {
    return Identifier::peek(currentToken);
}

FuncArg * FuncArg::parse(ParseCtx & parseCtx) {
    // Parse the identifier and skip any newlines that follow:
    Identifier * ident = Identifier::parse(parseCtx);
    WC_GUARD(ident, nullptr);
    parseCtx.skipNewlines();
    
    // Expect ':' following the identifier
    if (parseCtx.tok()->type != TokenType::kColon) {
        parseCtx.error("expect ':' following argument name for function argument!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Skip ':'
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // Parse the data type
    Type * type = Type::parse(parseCtx);
    WC_GUARD(type, nullptr);
    
    // Success, return the node:
    return WC_NEW_AST_NODE(parseCtx, FuncArg, *type, *ident);
}

FuncArg::FuncArg(Type & type, Identifier & ident) :
    mType(type),
    mIdent(ident)
{
    mType.mParent = this;
    mIdent.mParent = this;
}

void FuncArg::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & FuncArg::getStartToken() const {
    return mType.getStartToken();
}

const Token & FuncArg::getEndToken() const {
    return mIdent.getEndToken();
}

const char * FuncArg::name() const {
    return mIdent.name();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
