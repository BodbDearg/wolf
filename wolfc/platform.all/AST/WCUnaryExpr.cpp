#include "WCUnaryExpr.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// UnaryExpr
//-----------------------------------------------------------------------------

bool UnaryExpr::peek(const Token * currentToken) {
    /* 
    -PrimaryExpr
    +PrimaryExpr
    */
    if (currentToken->type == TokenType::kMinus || currentToken->type == TokenType::kPlus) {
        return PrimaryExpr::peek(currentToken + 1);
    }
    
    /* (AssignExpr) */
    if (currentToken->type == TokenType::kLParen) {
        return AssignExpr::peek(currentToken + 1);
    }
    
    /* PrimaryExpr */
    return PrimaryExpr::peek(currentToken);
}

UnaryExpr * UnaryExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    switch (currentToken->type) {
        /* -IntLit */
        case TokenType::kMinus: {
            const Token * minusTok = currentToken;
            ++currentToken;     // Skip '-'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprNegPrimary, *expr, *minusTok);
        }   break;
            
        /* +PrimaryExpr */
        case TokenType::kPlus: {
            const Token * plusTok = currentToken;
            ++currentToken;     // Skip '+'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprPosPrimary, *expr, *plusTok);
        }   break;
            
        /* (AssignExpr) */
        case TokenType::kLParen: {
            const Token * lparenTok = currentToken;
            ++currentToken;     // Skip '('
            AssignExpr * expr = AssignExpr::parse(currentToken, alloc);
            
            if (currentToken->type != TokenType::kRParen) {
                parseError(*currentToken,
                           "Expected closing ')' to match '(' at line %zu and column %zu!",
                           lparenTok->startLine + 1,
                           lparenTok->startCol + 1);
                
                return nullptr;
            }
            
            const Token * rparenTok = currentToken;
            ++currentToken;     // Skip ')'
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprParen, *expr, *lparenTok, *rparenTok);
        }   break;
            
        /* PrimaryExpr */
        default: {
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprPrimary, *expr);
        }   break;
    }
    
    return nullptr;     // Should never reach here
}

//-----------------------------------------------------------------------------
// UnaryExprPrimary
//-----------------------------------------------------------------------------

UnaryExprPrimary::UnaryExprPrimary(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
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

const DataType & UnaryExprPrimary::dataType() const {
    return mExpr.dataType();
}

llvm::Value * UnaryExprPrimary::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprPrimary::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprNegPrimary
//-----------------------------------------------------------------------------

UnaryExprNegPrimary::UnaryExprNegPrimary(PrimaryExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & UnaryExprNegPrimary::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprNegPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprNegPrimary::isLValue() const {
    return false;
}

const DataType & UnaryExprNegPrimary::dataType() const {
    return mExpr.dataType();
}

llvm::Value * UnaryExprNegPrimary::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprNegPrimary::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: support more types
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Unary '-' operator only supports 'int' datatype, not '%s'!", exprType.name());
        return nullptr;
    }
    
    return cgCtx.irBuilder.CreateNeg(mExpr.codegenExprEval(cgCtx));
}

//-----------------------------------------------------------------------------
// UnaryExprPosPrimary
//-----------------------------------------------------------------------------

UnaryExprPosPrimary::UnaryExprPosPrimary(PrimaryExpr & expr, const Token & startToken) :
    UnaryExprPrimary(expr),
    mStartToken(startToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & UnaryExprPosPrimary::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprPosPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprPosPrimary::isLValue() const {
    return false;
}

const DataType & UnaryExprPosPrimary::dataType() const {
    return mExpr.dataType();
}

llvm::Value * UnaryExprPosPrimary::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprPosPrimary::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: support more types
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Unary '+' operator only supports 'int' datatype, not '%s'!", exprType.name());
        return nullptr;
    }
    
    return UnaryExprPrimary::codegenExprEval(cgCtx);
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

const Token & UnaryExprParen::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprParen::getEndToken() const {
    return mEndToken;
}

bool UnaryExprParen::isLValue() const {
    return mExpr.isLValue();
}

const DataType & UnaryExprParen::dataType() const {
    return mExpr.dataType();
}

llvm::Value * UnaryExprParen::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprParen::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

WC_END_NAMESPACE
