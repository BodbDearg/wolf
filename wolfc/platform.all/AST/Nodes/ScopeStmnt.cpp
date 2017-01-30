#include "ScopeStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Assert.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool ScopeStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kScope;
}

ScopeStmnt * ScopeStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'scope' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("'scope' statement expected!");
        return nullptr;
    }
    
    // Skip the 'scope' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the body scope:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    
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
