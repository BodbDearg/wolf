#include "BreakStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BreakStmnt
//-----------------------------------------------------------------------------
bool BreakStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kBreak;
}

BreakStmnt * BreakStmnt::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // Check the basics
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected break statement!");
        return nullptr;
    }
    
    // Consume 'break' and save token for later:
    const Token * breakTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // See whether 'if' or 'unless' follow, in which case the 'break' statement is conditional:
    if (parseCtx.tok()->type == TokenType::kIf || parseCtx.tok()->type == TokenType::kUnless) {
        // Parse the condition token:
        const Token * condTok = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the condition assign expression:
        AssignExpr * condExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(condExpr, nullptr);
        
        // 'break' with a condition:
        return WC_NEW_AST_NODE(parseCtx, BreakStmntWithCond, *breakTok, *condTok, *condExpr);
    }
    
    // 'break' without a condition:
    return WC_NEW_AST_NODE(parseCtx, BreakStmntNoCond, *breakTok);
}

BreakStmnt::BreakStmnt(const Token & breakToken) : mBreakToken(breakToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & BreakStmnt::getStartToken() const {
    return mBreakToken;
}

bool BreakStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

//-----------------------------------------------------------------------------
// BreakStmntNoCond
//-----------------------------------------------------------------------------
BreakStmntNoCond::BreakStmntNoCond(const Token & breakToken) : BreakStmnt(breakToken) {
    WC_EMPTY_FUNC_BODY();
}

void BreakStmntNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & BreakStmntNoCond::getEndToken() const {
    return mBreakToken;
}

//-----------------------------------------------------------------------------
// BreakStmntWithCond
//-----------------------------------------------------------------------------
BreakStmntWithCond::BreakStmntWithCond(const Token & breakToken,
                                       const Token & condToken,
                                       AssignExpr & condExpr)
:
    BreakStmnt(breakToken),
    mCondToken(condToken),
    mCondExpr(condExpr)
{
    mCondExpr.mParent = this;
}

void BreakStmntWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & BreakStmntWithCond::getEndToken() const {
    return mCondExpr.getEndToken();
}

bool BreakStmntWithCond::isIfCondInverted() const {
    return mCondToken.type == TokenType::kUnless;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
