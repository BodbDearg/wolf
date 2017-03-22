//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "PrefixExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "PostfixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PrefixExpr
//-----------------------------------------------------------------------------
bool PrefixExpr::peek(const Token * currentToken) {
    /*
        - PrefixExpr
        + PrefixExpr
        & PrefixExpr
        $ PrefixExpr
        ^ PrefixExpr
    */
    TokenType currentTokenType = currentToken->type;
    
    if (currentTokenType == TokenType::kMinus ||
        currentTokenType == TokenType::kPlus ||
        currentTokenType == TokenType::kAmpersand ||
        currentTokenType == TokenType::kDollar ||
        currentTokenType == TokenType::kHat)
    {
        ++currentToken;
        WC_PARSER_SKIP_NEWLINE_TOKENS(currentToken);
        return PrefixExpr::peek(currentToken);
    }
    
    /* PostfixExpr */
    return PostfixExpr::peek(currentToken);
}

PrefixExpr * PrefixExpr::parse(ParseCtx & parseCtx) {
    switch (parseCtx.tok()->type) {
        /* - PostfixExpr */
        case TokenType::kMinus: {
            // Skip '-' and any newlines that follow
            const Token * minusTok = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the operand and create the AST node
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprMinus, *minusTok, *expr);
        }   break;
            
        /* + PostfixExpr */
        case TokenType::kPlus: {
            // Skip '+' and any newlines that follow
            const Token * plusTok = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the operand and create the AST node
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprPlus, *plusTok, *expr);
        }   break;
            
        /* & PostfixExpr */
        case TokenType::kAmpersand: {
            // Skip '&' and any newlines that follow
            const Token * ampersandTok = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the operand and create the AST node
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprAddrOf, *ampersandTok, *expr);
        }   break;
            
        /* $ PostfixExpr */
        case TokenType::kDollar: {
            // Skip '$' and any newlines that follow
            const Token * dollarTok = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the operand and create the AST node
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprPtrDeref, *dollarTok, *expr);
        }   break;
            
        /* ^ PostfixExpr */
        case TokenType::kHat: {
            // Skip '^' and any newlines that follow
            const Token * hatTok = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the operand and create the AST node
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprPtrDenull, *hatTok, *expr);
        }   break;
            
        /* PostfixExpr */
        default: {
            PostfixExpr * expr = PostfixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprNoOp, *expr);
        }   break;
    }
    
    return nullptr;     // Should never reach here
}

//-----------------------------------------------------------------------------
// PrefixExprNoOp
//-----------------------------------------------------------------------------
PrefixExprNoOp::PrefixExprNoOp(PostfixExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrefixExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrefixExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrefixExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PrefixExprWithUnaryOp
//-----------------------------------------------------------------------------
PrefixExprWithUnaryOp::PrefixExprWithUnaryOp(const Token & startToken, PrefixExpr & expr) :
    mStartToken(startToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & PrefixExprWithUnaryOp::getStartToken() const {
    return mStartToken;
}

const Token & PrefixExprWithUnaryOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PrefixExprPlus
//-----------------------------------------------------------------------------
PrefixExprPlus::PrefixExprPlus(const Token & startToken, PrefixExpr & expr) :
    PrefixExprWithUnaryOp(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void PrefixExprPlus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PrefixExprMinus
//-----------------------------------------------------------------------------
PrefixExprMinus::PrefixExprMinus(const Token & startToken, PrefixExpr & expr) :
    PrefixExprWithUnaryOp(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void PrefixExprMinus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PrefixExprAddrOf
//-----------------------------------------------------------------------------
PrefixExprAddrOf::PrefixExprAddrOf(const Token & startToken, PrefixExpr & expr) :
    PrefixExprWithUnaryOp(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void PrefixExprAddrOf::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PrefixExprPtrDeref
//-----------------------------------------------------------------------------
PrefixExprPtrDeref::PrefixExprPtrDeref(const Token & startToken, PrefixExpr & expr) :
    PrefixExprWithUnaryOp(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}
    
void PrefixExprPtrDeref::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PrefixExprPtrDenull
//-----------------------------------------------------------------------------
PrefixExprPtrDenull::PrefixExprPtrDenull(const Token & startToken, PrefixExpr & expr) :
    PrefixExprWithUnaryOp(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void PrefixExprPtrDenull::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
