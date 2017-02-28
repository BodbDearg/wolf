//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "LoopStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LoopStmnt
//-----------------------------------------------------------------------------
bool LoopStmnt::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kLoop;
}

LoopStmnt * LoopStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'loop' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("'loop' statement expected!");
        return nullptr;
    }
    
    // Skip the 'loop' token and save location, also skip any
    // newlines which might follow:
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the body scope and skip any newlines that follow:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    parseCtx.skipNewlines();
    
    // See if the current token is 'repeat', if it is then we have a loop/repeat statement
    if (parseCtx.tok()->type == TokenType::kRepeat) {
        // Loop block with a condition.
        // Skip the 'repeat' keyword and any newlines that follow:
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Now grab the condition type.
        // Skip the condition keyword and any newlines that follow:
        const Token * condTypeToken = parseCtx.tok();
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        if (condTypeToken->type != TokenType::kWhile && condTypeToken->type != TokenType::kUntil) {
            parseCtx.error("'while' or 'until' expected following 'repeat' token!");
            return nullptr;
        }
        
        // Now expect an assign expression as the condition
        AssignExpr * loopCondExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(loopCondExpr, nullptr);
        
        // All done, return the parsed expression:
        return WC_NEW_AST_NODE(parseCtx,
                               LoopStmntWithCond,
                               *bodyScope,
                               *startToken,
                               *condTypeToken,
                               *loopCondExpr);
    }
    else {
        // Conditionless loop block: should be terminated by an 'end' token:
        if (parseCtx.tok()->type != TokenType::kEnd) {
            parseCtx.error("'end' expected to terminate 'loop' block!");
            return nullptr;
        }
        
        // Skip 'end' token and save location
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Done: return the parsed statement
        return WC_NEW_AST_NODE(parseCtx, LoopStmntNoCond, *bodyScope, *startToken, *endToken);
    }
}

LoopStmnt::LoopStmnt(Scope & bodyScope, const Token & startToken) :
    mBodyScope(bodyScope),
    mStartToken(startToken)
{
    mBodyScope.mParent = this;
}

const Token & LoopStmnt::getStartToken() const {
    return mStartToken;
}

bool LoopStmnt::allCodepathsHaveUncondRet() const {
    return mBodyScope.allCodepathsHaveUncondRet();
}

//-----------------------------------------------------------------------------
// LoopStmntNoCond
//-----------------------------------------------------------------------------
LoopStmntNoCond::LoopStmntNoCond(Scope & bodyScope, const Token & startToken, const Token & endToken) :
    LoopStmnt(bodyScope, startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void LoopStmntNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LoopStmntNoCond::getEndToken() const {
    return mEndToken;
}

//-----------------------------------------------------------------------------
// LoopStmntWithCond
//-----------------------------------------------------------------------------
LoopStmntWithCond::LoopStmntWithCond(Scope & bodyScope,
                                     const Token & startToken,
                                     const Token & condTypeToken,
                                     AssignExpr & loopCondExpr)
:
    LoopStmnt(bodyScope, startToken),
    mCondTypeToken(condTypeToken),
    mLoopCondExpr(loopCondExpr)
{
    WC_ASSERT(mCondTypeToken.type == TokenType::kWhile || mCondTypeToken.type == TokenType::kUntil);
    mLoopCondExpr.mParent = this;
}

void LoopStmntWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & LoopStmntWithCond::getEndToken() const {
    return mLoopCondExpr.getEndToken();
}

bool LoopStmntWithCond::isLoopCondInversed() const {
    return mCondTypeToken.type == TokenType::kUntil;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
