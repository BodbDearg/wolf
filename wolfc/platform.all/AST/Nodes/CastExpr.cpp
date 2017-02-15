#include "CastExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "PrimaryExpr.hpp"
#include "Type.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CastExpr
//-----------------------------------------------------------------------------
bool CastExpr::peek(const Token * currentToken) {
    return PrimaryExpr::peek(currentToken) || currentToken->type == TokenType::kCast;
}

CastExpr * CastExpr::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // See if 'cast' follows:
    if (parseCtx.tok()->type == TokenType::kCast) {
        // Casting expression: skip 'cast'
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect opening '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'cast' for 'cast()' operator!");
            return nullptr;
        }
        
        // Skip '('
        parseCtx.nextTok();
        
        // Parse the initial assign expression:
        AssignExpr * expr = AssignExpr::parse(parseCtx);
        WC_GUARD(expr, nullptr);
        
        // Expect keyword 'to':
        if (parseCtx.tok()->type != TokenType::kTo) {
            parseCtx.error("Expecting keyword 'to' following the expression to cast inside 'cast()' operator!");
            return nullptr;
        }
        
        // Skip 'to'
        parseCtx.nextTok();
        
        // Parse the type to cast to:
        Type * type = Type::parse(parseCtx);
        WC_GUARD(type, nullptr);
        
        // Expect closing ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expecting closing ')' for 'cast()' operator!");
            return nullptr;
        }
        
        // Skip ')'
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Return the parsed node:
        return WC_NEW_AST_NODE(parseCtx,
                               CastExprCast,
                               *startToken,
                               *expr,
                               *type,
                               *endToken);
    }
    
    // Normal expression that does no cast:
    PrimaryExpr * primaryExpr = PrimaryExpr::parse(parseCtx);
    WC_GUARD(primaryExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, CastExprNoCast, *primaryExpr);
}

//-----------------------------------------------------------------------------
// CastExprNoCast
//-----------------------------------------------------------------------------
CastExprNoCast::CastExprNoCast(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void CastExprNoCast::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & CastExprNoCast::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CastExprNoCast::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// CastExprCast
//-----------------------------------------------------------------------------
CastExprCast::CastExprCast(const Token & startToken,
                           AssignExpr & expr,
                           Type & type,
                           const Token & endToken)
:
    mStartToken(startToken),
    mExpr(expr),
    mType(type),
    mEndToken(endToken)
{
    mExpr.mParent = this;
    mType.mParent = this;
}

void CastExprCast::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & CastExprCast::getStartToken() const {
    return mStartToken;
}

const Token & CastExprCast::getEndToken() const {
    return mEndToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
