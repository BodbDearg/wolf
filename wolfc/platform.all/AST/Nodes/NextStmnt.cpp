#include "NextStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NextStmnt
//-----------------------------------------------------------------------------
bool NextStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kNext;
}

NextStmnt * NextStmnt::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // Check the basics
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected next statement!");
        return nullptr;
    }
    
    // Consume 'next' and save token for later:
    const Token * nextTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // See whether 'if' or 'unless' follow, in which case the 'next' statement is conditional:
    if (parseCtx.tok()->type == TokenType::kIf || parseCtx.tok()->type == TokenType::kUnless) {
        // Parse the condition token:
        const Token * condTok = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the condition assign expression:
        AssignExpr * condExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(condExpr, nullptr);
        
        // 'next' with a condition:
        return WC_NEW_AST_NODE(parseCtx, NextStmntWithCond, *nextTok, *condTok, *condExpr);
    }    
    
    // 'next' without a condition:
    return WC_NEW_AST_NODE(parseCtx, NextStmntNoCond, *nextTok);
}

NextStmnt::NextStmnt(const Token & nextToken) : mNextToken(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & NextStmnt::getStartToken() const {
    return mNextToken;
}

//-----------------------------------------------------------------------------
// NextStmntNoCond
//-----------------------------------------------------------------------------
NextStmntNoCond::NextStmntNoCond(const Token & nextToken) : NextStmnt(nextToken) {
    WC_EMPTY_FUNC_BODY();
}

void NextStmntNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NextStmntNoCond::getEndToken() const {
    return mNextToken;
}

bool NextStmntNoCond::allCodepathsHaveUncondRet() const {
    return true;
}

//-----------------------------------------------------------------------------
// NextStmntWithCond
//-----------------------------------------------------------------------------
NextStmntWithCond::NextStmntWithCond(const Token & nextToken,
                                     const Token & condToken,
                                     AssignExpr & condExpr)
:
    NextStmnt(nextToken),
    mCondToken(condToken),
    mCondExpr(condExpr)
{
    mCondExpr.mParent = this;
}

void NextStmntWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & NextStmntWithCond::getEndToken() const {
    return mCondExpr.getEndToken();
}

bool NextStmntWithCond::allCodepathsHaveUncondRet() const {
    return false;
}

bool NextStmntWithCond::isIfCondInverted() const {
    return mCondToken.type == TokenType::kUnless;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
