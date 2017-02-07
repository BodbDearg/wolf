#include "RandExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// RandExpr
//-----------------------------------------------------------------------------
bool RandExpr::peek(const Token * tokenPtr) {
    return  tokenPtr->type == TokenType::kRand ||
            tokenPtr->type == TokenType::kSRand;
}

RandExpr * RandExpr::parse(ParseCtx & parseCtx) {
    TokenType tokType = parseCtx.tok()->type;
    
    if (tokType == TokenType::kRand) {
        // rand() call: consume 'rand' and save start token
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'rand'!");
            return nullptr;
        }
        
        parseCtx.nextTok();     // Consume '('
        
        // Expect ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expect ')' following 'rand('!");
            return nullptr;
        }
        
        // Consume ')', save end token and then return parsed node
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        return WC_NEW_AST_NODE(parseCtx, RandExprRand, *startToken, *endToken);
    }
    else if (tokType == TokenType::kSRand) {
        // srand() call: consume 'srand' and save start token
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'srand'!");
            return nullptr;
        }
        
        parseCtx.nextTok();     // Consume '('
        
        // Parse the inner assign expression for the seed
        AssignExpr * seedExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(seedExpr, nullptr);
        
        // Expect ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expect ')' to close 'srand()' call!");
            return nullptr;
        }
        
        // Consume ')', save end token and then return parsed node
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        return WC_NEW_AST_NODE(parseCtx, RandExprSRand, *startToken, *seedExpr, *endToken);
    }
    
    parseCtx.error("Expected 'rand' or 'srand' token!");
    return nullptr;
}

RandExpr::RandExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & RandExpr::getStartToken() const {
    return mStartToken;
}

const Token & RandExpr::getEndToken() const {
    return mEndToken;
}

//-----------------------------------------------------------------------------
// RandExprRand
//-----------------------------------------------------------------------------
RandExprRand::RandExprRand(const Token & startToken, const Token & endToken) :
    RandExpr(startToken, endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void RandExprRand::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// RandExprSRand
//-----------------------------------------------------------------------------
RandExprSRand::RandExprSRand(const Token & startToken,
                             AssignExpr & seedExpr,
                             const Token & endToken)
:
    RandExpr(startToken, endToken),
    mSeedExpr(seedExpr)
{
    mSeedExpr.mParent = this;
}

void RandExprSRand::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
