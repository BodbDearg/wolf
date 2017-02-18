#include "PostfixExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "CastExpr.hpp"
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
    
    // Continue parsing and wrapping function calls and array lookups while we can
    while (parseCtx.tok()->type == TokenType::kLParen ||
           parseCtx.tok()->type == TokenType::kLBrack)
    {
        // See if function call follows:
        if (parseCtx.tok()->type == TokenType::kLParen) {
            // Save and consume the '(' token as well as newlines following:
            const Token * openingParen = parseCtx.tok();
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Start parsing the arg list
            std::vector<AssignExpr*> argExprs;

            while (AssignExpr::peek(parseCtx.tok())) {
                // Parse the arg and save if it was parsed ok,
                // also skip any newlines that follow:
                AssignExpr * argExpr = AssignExpr::parse(parseCtx);
                parseCtx.skipNewlines();
                
                if (argExpr) {
                    argExprs.push_back(argExpr);
                }
                
                // If a comma does not follow then we are done
                if (parseCtx.tok()->type != TokenType::kComma) {
                    break;
                }
                
                // Otherwise continue parsing and skip ',' as well
                // as any newlines which might follow:
                parseCtx.nextTok();
                parseCtx.skipNewlines();
            }
            
            // Expect ')'
            const Token * endToken = parseCtx.tok();
            
            if (endToken->type != TokenType::kRParen) {
                // Failed, issue an error and return a null pointer
                parseCtx.error("Expected ')' !");
                return nullptr;
            }
            else {
                // Save and skip the ')'
                const Token * closingParen = parseCtx.tok();
                parseCtx.nextTok();
                
                // Save the parsed node:
                outerPostfixExpr = WC_NEW_AST_NODE(parseCtx,
                                                   PostfixExprFuncCall,
                                                   *outerPostfixExpr,
                                                   *openingParen,
                                                   argExprs,
                                                   *closingParen);
            }
        }
        else {
            // An array lookup follows, skip the '[' and any newlines that follow.
            // Expect '[' to be here based on previous if() failing - see while loop.
            WC_ASSERT(parseCtx.tok()->type == TokenType::kLBrack);
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Parse the assign expression for the array index and skip any newlines that follow
            AssignExpr * arrayIndexExpr = AssignExpr::parse(parseCtx);
            WC_GUARD(arrayIndexExpr, nullptr);
            parseCtx.skipNewlines();
            
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
PostfixExprFuncCall::PostfixExprFuncCall(PostfixExpr & operandExpr,
                                         const Token & callOpeningParen,
                                         std::vector<AssignExpr*> & argExprs,
                                         const Token & callClosingParen)
:
    mOperandExpr(operandExpr),
    mCallOpeningParen(callOpeningParen),
    mArgExprs(),
    mCallClosingParen(callClosingParen)
{
    mOperandExpr.mParent = this;
    mArgExprs.reserve(argExprs.size());
    
    for (AssignExpr * argExpr : argExprs) {
        argExpr->mParent = this;
        mArgExprs.push_back(argExpr);
    }
}

void PostfixExprFuncCall::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprFuncCall::getStartToken() const {
    return mOperandExpr.getStartToken();
}

const Token & PostfixExprFuncCall::getEndToken() const {
    return mCallClosingParen;
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
