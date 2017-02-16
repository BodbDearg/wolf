#include "Scope.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "Stmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Scope * Scope::parse(ParseCtx & parseCtx) {
    // Skip any newlines that follow which the scope starts with:
    parseCtx.skipNewlines();
    
    // Save start token:
    const Token * startToken = parseCtx.tok();
    
    // Parse all statements we can:
    std::vector<Stmnt*> stmnts;
    
    while (Stmnt::peek(parseCtx.tok())) {
        // Parse the statement
        Stmnt * stmnt = Stmnt::parse(parseCtx);
        WC_GUARD(stmnt, nullptr);
        stmnts.push_back(stmnt);
        
        // Skip any newlines that follow
        parseCtx.skipNewlines();
    }
    
    // Return the parsed scope
    return WC_NEW_AST_NODE(parseCtx, Scope, *startToken, std::move(stmnts));
}

Scope::Scope(const Token & startToken, std::vector<Stmnt*> && stmnts)
:
    mStartToken(startToken),
    mStmnts(stmnts)
{
    for (Stmnt * stmnt : mStmnts) {
        stmnt->mParent = this;
    }
}

void Scope::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Scope::getStartToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.front()->getStartToken();
    }
    
    return mStartToken;
}

const Token & Scope::getEndToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.back()->getEndToken();
    }
    
    return mStartToken;
}

bool Scope::allCodepathsHaveUncondRet() const {
    for (Stmnt * stmnt : mStmnts) {
        if (stmnt->allCodepathsHaveUncondRet()) {
            return true;
        }
    }
    
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
