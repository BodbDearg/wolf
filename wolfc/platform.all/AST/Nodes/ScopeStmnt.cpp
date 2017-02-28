//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ScopeStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool ScopeStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kScope;
}

ScopeStmnt * ScopeStmnt::parse(ParseCtx & parseCtx) {
    // Sanity check
    if (!peek(parseCtx.tok())) {
        parseCtx.error("'scope' statement expected!");
        return nullptr;
    }
    
    // Parse the initial 'scope' keyword and save it's location.
    // Also skip any newlines that follow:
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the body scope and skip any newlines that follow:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    parseCtx.skipNewlines();
    
    // Must be terminated by an 'end' token
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate 'scope' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(parseCtx, ScopeStmnt, *startToken, *bodyScope, *endToken);
}

ScopeStmnt::ScopeStmnt(const Token & startToken, Scope & bodyScope, const Token & endToken) :
    mStartToken(startToken),
    mBodyScope(bodyScope),
    mEndToken(endToken)
{
    mBodyScope.mParent = this;
}

void ScopeStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ScopeStmnt::getStartToken() const {
    return mStartToken;
}

const Token & ScopeStmnt::getEndToken() const {
    return mEndToken;
}

bool ScopeStmnt::allCodepathsHaveUncondRet() const {
    return mBodyScope.allCodepathsHaveUncondRet();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
