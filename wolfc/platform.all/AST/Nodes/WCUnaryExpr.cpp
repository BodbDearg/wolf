#include "WCUnaryExpr.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCPostfixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// UnaryExpr
//-----------------------------------------------------------------------------
bool UnaryExpr::peek(const Token * currentToken) {
    /* 
    -PostfixExpr
    +PostfixExpr
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
        /* -PostfixExpr */
        case TokenType::kMinus: {
            const Token * minusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '-'
            PostfixExpr * expr = PostfixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprMinus, *expr, *minusTok);
        }   break;
            
        /* +PostfixExpr */
        case TokenType::kPlus: {
            const Token * plusTok = parseCtx.tok();
            parseCtx.nextTok();     // Skip '+'
            PostfixExpr * expr = PostfixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprPlus, *expr, *plusTok);
        }   break;
            
        /* (AssignExpr) */
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
            return WC_NEW_AST_NODE(parseCtx, UnaryExprParen, *expr, *lparenTok, *rparenTok);
        }   break;
            
        /* PostfixExpr */
        default: {
            PostfixExpr * expr = PostfixExpr::parse(parseCtx);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, UnaryExprPrimary, *expr);
        }   break;
    }
    
    return nullptr;     // Should never reach here
}

//-----------------------------------------------------------------------------
// UnaryExprPrimary
//-----------------------------------------------------------------------------
UnaryExprPrimary::UnaryExprPrimary(PostfixExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void UnaryExprPrimary::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & UnaryExprPrimary::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & UnaryExprPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprPrimary::isLValue() const {
    return mExpr.isLValue();
}

bool UnaryExprPrimary::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & UnaryExprPrimary::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// UnaryExprPlusMinusBase
//-----------------------------------------------------------------------------
UnaryExprPlusMinusBase::UnaryExprPlusMinusBase(PostfixExpr & expr, const Token & startToken) :
    UnaryExprPrimary(expr),
    mStartToken(startToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & UnaryExprPlusMinusBase::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprPlusMinusBase::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprPlusMinusBase::isLValue() const {
    return false;
}

#warning FIXME - Codegen
#if 0
llvm::Constant * UnaryExprPlusMinusBase::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Generate the code for the expression value first
    llvm::Constant * exprValue = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(exprValue, nullptr);
    
    // Now do the operation
    DataType & exprType = mExpr.dataType();
    return (exprType.*mCodegenConstUnaryOpFunc)(*this, *exprValue);
}
#endif

//-----------------------------------------------------------------------------
// UnaryExprPlus
//-----------------------------------------------------------------------------
UnaryExprPlus::UnaryExprPlus(PostfixExpr & expr, const Token & startToken) :
    UnaryExprPlusMinusBase(expr, startToken)
{
    WC_EMPTY_FUNC_BODY();
}

void UnaryExprPlus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// UnaryExprMinus
//-----------------------------------------------------------------------------
UnaryExprMinus::UnaryExprMinus(PostfixExpr & expr, const Token & startToken) :
    UnaryExprPlusMinusBase(expr, startToken)
{
    WC_EMPTY_FUNC_BODY();
}

void UnaryExprMinus::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// UnaryExprParen
//-----------------------------------------------------------------------------
UnaryExprParen::UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken) :
    mExpr(expr),
    mStartToken(startToken),
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

bool UnaryExprParen::isLValue() const {
    return mExpr.isLValue();
}

bool UnaryExprParen::isConstExpr() const {
    return mExpr.isLValue();
}

const DataType & UnaryExprParen::dataType() const {
    return mExpr.dataType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
