#include "PrefixExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
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
    */
    if (currentToken->type == TokenType::kMinus || currentToken->type == TokenType::kPlus) {
        return PostfixExpr::peek(currentToken + 1);
    }
    
    /* (AssignExpr) */
    if (currentToken->type == TokenType::kLParen) {
        return AssignExpr::peek(currentToken + 1);
    }
    
    /* PrimaryExpr */
    return PostfixExpr::peek(currentToken);
}

PrefixExpr * PrefixExpr::parse(ParseCtx & parseCtx) {
    switch (parseCtx.tok()->type) {
        /* - PostfixExpr */
        case TokenType::kMinus: {
            const Token * minusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '-'
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprMinus, *minusTok, *expr);
        }   break;
            
        /* + PostfixExpr */
        case TokenType::kPlus: {
            const Token * plusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '+'
            PrefixExpr * expr = PrefixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprPlus, *plusTok, *expr);
        }   break;
            
        /* ( AssignExpr ) */
        case TokenType::kLParen: {
            const Token * lparenTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '('
            AssignExpr * expr = AssignExpr::parse(parseCtx);
            
            if (parseCtx.tok()->type != TokenType::kRParen) {
                parseCtx.error("Expected closing ')' to match '(' at line %zu and column %zu!",
                               lparenTok->startLine + 1,
                               lparenTok->startCol + 1);
                
                return nullptr;
            }
            
            const Token * rparenTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip ')'
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, PrefixExprParen, *lparenTok, *expr, *rparenTok);
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
// PrefixExprPlusMinusBase
//-----------------------------------------------------------------------------
PrefixExprPlusMinusBase::PrefixExprPlusMinusBase(const Token & startToken, PrefixExpr & expr) :
    mStartToken(startToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & PrefixExprPlusMinusBase::getStartToken() const {
    return mStartToken;
}

const Token & PrefixExprPlusMinusBase::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PrefixExprPlus
//-----------------------------------------------------------------------------
PrefixExprPlus::PrefixExprPlus(const Token & startToken, PrefixExpr & expr) :
    PrefixExprPlusMinusBase(startToken, expr)
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
    PrefixExprPlusMinusBase(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void PrefixExprMinus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PrefixExprParen
//-----------------------------------------------------------------------------
PrefixExprParen::PrefixExprParen(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

void PrefixExprParen::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrefixExprParen::getStartToken() const {
    return mStartToken;
}

const Token & PrefixExprParen::getEndToken() const {
    return mEndToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE