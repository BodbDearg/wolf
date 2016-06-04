#include "WCUnaryExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
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
    (BinaryExpr)
    */
    if (currentToken->type == TokenType::kMinus ||
        currentToken->type == TokenType::kPlus ||
        currentToken->type == TokenType::kLParen)
    {
        return PrimaryExpr::peek(currentToken + 1);
    }
    
    /* PrimaryExpr */
    return PrimaryExpr::peek(currentToken);
}

UnaryExpr * UnaryExpr::parse(const Token *& currentToken) {
    switch (currentToken->type) {
        /* -IntLit */
        case TokenType::kMinus: {
            const Token * minusTok = currentToken;
            ++currentToken;     // Skip '-'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            return new UnaryExprNegPrimary(*minusTok, *expr);
        }   break;
            
        /* +PrimaryExpr */
        case TokenType::kPlus: {
            const Token * plusTok = currentToken;
            ++currentToken;     // Skip '+'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            return new UnaryExprPosPrimary(*plusTok, *expr);
        }   break;
            
        /* (BinaryExpr) */
        case TokenType::kLParen: {
            const Token * lparenTok = currentToken;
            ++currentToken;     // Skip '('
            BinaryExpr * expr = BinaryExpr::parse(currentToken);
            
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
            return new UnaryExprParen(*lparenTok, *rparenTok, *expr);
        }   break;
            
        /* PrimaryExpr */
        default: {
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            return new UnaryExprPrimary(*expr);
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

llvm::Value * UnaryExprPrimary::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool UnaryExprPrimary::isLValue() const {
    return mExpr.isLValue();
}

const DataType & UnaryExprPrimary::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * UnaryExprPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprNegPrimary
//-----------------------------------------------------------------------------

UnaryExprNegPrimary::UnaryExprNegPrimary(const Token & startToken, PrimaryExpr & expr) :
    mStartToken(startToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & UnaryExprNegPrimary::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprNegPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * UnaryExprNegPrimary::generateCode(const CodegenCtx & cgCtx) {
    // TODO: support more types
    const DataType & exprType = mExpr.getDataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Unary '-' operator only supports 'int' datatype, not '%s'!", exprType.name());
        return nullptr;
    }
    
    return cgCtx.irBuilder.CreateNeg(mExpr.generateCode(cgCtx));
}

bool UnaryExprNegPrimary::isLValue() const {
    return false;
}

const DataType & UnaryExprNegPrimary::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * UnaryExprNegPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprPosPrimary
//-----------------------------------------------------------------------------

UnaryExprPosPrimary::UnaryExprPosPrimary(const Token & startToken, PrimaryExpr & expr) :
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

llvm::Value * UnaryExprPosPrimary::generateCode(const CodegenCtx & cgCtx) {
    // TODO: support more types
    const DataType & exprType = mExpr.getDataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Unary '+' operator only supports 'int' datatype, not '%s'!", exprType.name());
        return nullptr;
    }
    
    return UnaryExprPrimary::generateCode(cgCtx);
}

bool UnaryExprPosPrimary::isLValue() const {
    return false;
}

const DataType & UnaryExprPosPrimary::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * UnaryExprPosPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprParen
//-----------------------------------------------------------------------------

UnaryExprParen::UnaryExprParen(const Token & startToken, const Token & endToken, BinaryExpr & expr) :
    mStartToken(startToken),
    mEndToken(endToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & UnaryExprParen::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprParen::getEndToken() const {
    return mEndToken;
}

llvm::Value * UnaryExprParen::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool UnaryExprParen::isLValue() const {
    return mExpr.isLValue();
}

const DataType & UnaryExprParen::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * UnaryExprParen::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

WC_END_NAMESPACE
