#include "UnaryExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "PostfixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// UnaryExpr
//-----------------------------------------------------------------------------
bool UnaryExpr::peek(const Token * currentToken) {
    /* 
    - UnaryExpr
    + UnaryExpr
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

UnaryExpr * UnaryExpr::parse(ParseCtx & parseCtx) {
    switch (parseCtx.tok()->type) {
        /* - PostfixExpr */
        case TokenType::kMinus: {
            const Token * minusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '-'
            UnaryExpr * expr = UnaryExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprMinus, *minusTok, *expr);
        }   break;
            
        /* + PostfixExpr */
        case TokenType::kPlus: {
            const Token * plusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '+'
            UnaryExpr * expr = UnaryExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprPlus, *plusTok, *expr);
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
            return WC_NEW_AST_NODE(parseCtx, UnaryExprParen, *lparenTok, *expr, *rparenTok);
        }   break;
            
        /* PostfixExpr */
        default: {
            PostfixExpr * expr = PostfixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprNoOp, *expr);
        }   break;
    }
    
    return nullptr;     // Should never reach here
}

//-----------------------------------------------------------------------------
// UnaryExprNoOp
//-----------------------------------------------------------------------------
UnaryExprNoOp::UnaryExprNoOp(PostfixExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void UnaryExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & UnaryExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & UnaryExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

const DataType & UnaryExprNoOp::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// UnaryExprPlusMinusBase
//-----------------------------------------------------------------------------
UnaryExprPlusMinusBase::UnaryExprPlusMinusBase(const Token & startToken, UnaryExpr & expr) :
    mStartToken(startToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & UnaryExprPlusMinusBase::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprPlusMinusBase::getEndToken() const {
    return mExpr.getEndToken();
}

const DataType & UnaryExprPlusMinusBase::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// UnaryExprPlus
//-----------------------------------------------------------------------------
UnaryExprPlus::UnaryExprPlus(const Token & startToken, UnaryExpr & expr) :
    UnaryExprPlusMinusBase(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void UnaryExprPlus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// UnaryExprMinus
//-----------------------------------------------------------------------------
UnaryExprMinus::UnaryExprMinus(const Token & startToken, UnaryExpr & expr) :
    UnaryExprPlusMinusBase(startToken, expr)
{
    WC_EMPTY_FUNC_BODY();
}

void UnaryExprMinus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// UnaryExprParen
//-----------------------------------------------------------------------------
UnaryExprParen::UnaryExprParen(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

void UnaryExprParen::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & UnaryExprParen::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprParen::getEndToken() const {
    return mEndToken;
}

const DataType & UnaryExprParen::dataType() const {
    return mExpr.dataType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
