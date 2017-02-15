#include "PostfixExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "CastExpr.hpp"
#include "FuncCall.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PostfixExpr
//-----------------------------------------------------------------------------
bool PostfixExpr::peek(const Token * currentToken) {
    return CastExpr::peek(currentToken);
}

PostfixExpr * PostfixExpr::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // Parse the initial expression
    CastExpr * expr = CastExpr::parse(parseCtx);
    WC_GUARD(expr, nullptr);

    // Save the outermost postfix expression here:
    PostfixExpr * outerPostfixExpr = nullptr;

    // See if '++' or '--' follow:
    if (parseCtx.tok()->type == TokenType::kIncrement) {
        // Consume the '++' token and save
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();

        // Create outer postfix expr
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprInc, *expr, *endToken);
        WC_ASSERT(outerPostfixExpr);
    }
    else if (parseCtx.tok()->type == TokenType::kDecrement) {
        // Consume the '--' token and save
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();

        // Create outer postfix expr
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprDec, *expr, *endToken);
        WC_ASSERT(outerPostfixExpr);
    }
    else {
        // Basic postfix outer expression with no increment or decrement
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprNoOp, *expr);
        WC_ASSERT(outerPostfixExpr);
    }
    
    // Continue parsing and wrapping until one of these conditions breaks
    while (FuncCall::peek(parseCtx.tok()) ||
           parseCtx.tok()->type == TokenType::kLBrack)
    {
        // See if function call follows:
        if (FuncCall::peek(parseCtx.tok())) {
            FuncCall * funcCall = FuncCall::parse(parseCtx);
            WC_GUARD(funcCall, nullptr);
            outerPostfixExpr = WC_NEW_AST_NODE(parseCtx,
                                               PostfixExprFuncCall,
                                               *outerPostfixExpr,
                                               *funcCall);
            
            WC_ASSERT(outerPostfixExpr);
        }
        else {
            // Skip the '['. Expect '[' to be here based on previous if() failing - see while loop.
            WC_ASSERT(parseCtx.tok()->type == TokenType::kLBrack);
            parseCtx.nextTok();
            
            // Parse the assign expression for the array index
            AssignExpr * arrayIndexExpr = AssignExpr::parse(parseCtx);
            WC_GUARD(arrayIndexExpr, nullptr);
            
            // Expect a closing ']'
            const Token & endToken = *parseCtx.tok();
            
            if (endToken.type != TokenType::kRBrack) {
                parseCtx.error("Expected a closing ']' for array lookup expression!");
                return nullptr;
            }
            
            // Consume the ']' and create the outer postfix expression
            parseCtx.nextTok();
            outerPostfixExpr = WC_NEW_AST_NODE(parseCtx,
                                               PostfixExprArrayLookup,
                                               *outerPostfixExpr,
                                               *arrayIndexExpr,
                                               endToken);
            
            WC_ASSERT(outerPostfixExpr);
        }
    }
    
    return outerPostfixExpr;
}

//-----------------------------------------------------------------------------
// PostfixExprNoOp
//-----------------------------------------------------------------------------
PostfixExprNoOp::PostfixExprNoOp(CastExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PostfixExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PostfixExprIncDecBase
//-----------------------------------------------------------------------------
PostfixExprIncDecBase::PostfixExprIncDecBase(CastExpr & expr, const Token & endToken) :
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

const Token & PostfixExprIncDecBase::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprIncDecBase::getEndToken() const {
    return mEndToken;
}

//-----------------------------------------------------------------------------
// PostfixExprInc
//-----------------------------------------------------------------------------
PostfixExprInc::PostfixExprInc(CastExpr & expr, const Token & endToken) :
    PostfixExprIncDecBase(expr, endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void PostfixExprInc::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PostfixExprDec
//-----------------------------------------------------------------------------
PostfixExprDec::PostfixExprDec(CastExpr & expr, const Token & endToken) :
    PostfixExprIncDecBase(expr, endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void PostfixExprDec::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PostfixExprFuncCall
//-----------------------------------------------------------------------------
PostfixExprFuncCall::PostfixExprFuncCall(PostfixExpr & expr, FuncCall & funcCall) :
    mExpr(expr),
    mFuncCall(funcCall)
{
    mExpr.mParent = this;
    mFuncCall.mParent = this;
}

void PostfixExprFuncCall::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprFuncCall::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprFuncCall::getEndToken() const {
    return mFuncCall.getEndToken();
}

//-----------------------------------------------------------------------------
// PostfixExprArrayLookup
//-----------------------------------------------------------------------------
PostfixExprArrayLookup::PostfixExprArrayLookup(PostfixExpr & arrayExpr,
                                               AssignExpr & indexExpr,
                                               const Token & endToken)
:
    mArrayExpr(arrayExpr),
    mIndexExpr(indexExpr),
    mEndToken(endToken)
{
    mArrayExpr.mParent = this;
    mIndexExpr.mParent = this;
}

void PostfixExprArrayLookup::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprArrayLookup::getStartToken() const {
    return mArrayExpr.getStartToken();
}

const Token & PostfixExprArrayLookup::getEndToken() const {
    return mEndToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
