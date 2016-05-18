#include "WCUnaryExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

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
            ++currentToken; // Skip '-'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            return new UnaryExprNegPrimary(*expr);
        }   break;
            
        /* +PrimaryExpr */
        case TokenType::kPlus: {
            ++currentToken; // Skip '+'
            PrimaryExpr * expr = PrimaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            return new UnaryExprPosPrimary(*expr);
        }   break;
            
        /* (BinaryExpr) */
        case TokenType::kLParen: {
            const Token * lparenToken = currentToken;
            ++currentToken; // Skip '('
            
            BinaryExpr * expr = BinaryExpr::parse(currentToken);
            
            if (currentToken->type != TokenType::kRParen) {
                error(*currentToken,
                      "Expected closing ')' to match '(' at line %zu and column %zu!",
                      lparenToken->srcLine,
                      lparenToken->srcCol);
                
                return nullptr;
            }
            
            ++currentToken; // Skip ')'
            WC_GUARD(expr, nullptr);
            return new UnaryExprParen(*expr);
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

UnaryExprPrimary::UnaryExprPrimary(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

llvm::Value * UnaryExprPrimary::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool UnaryExprPrimary::isLValue() const {
    return mExpr.isLValue();
}

llvm::Value * UnaryExprPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

UnaryExprNegPrimary::UnaryExprNegPrimary(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

llvm::Value * UnaryExprNegPrimary::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.CreateNeg(mExpr.generateCode(cgCtx));
}

bool UnaryExprNegPrimary::isLValue() const {
    return false;
}

llvm::Value * UnaryExprNegPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

UnaryExprPosPrimary::UnaryExprPosPrimary(PrimaryExpr & expr) : UnaryExprPrimary(expr) {
    WC_EMPTY_FUNC_BODY();
}

bool UnaryExprPosPrimary::isLValue() const {
    return false;
}

llvm::Value * UnaryExprPosPrimary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

UnaryExprParen::UnaryExprParen(BinaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}
    
llvm::Value * UnaryExprParen::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool UnaryExprParen::isLValue() const {
    return mExpr.isLValue();
}

llvm::Value * UnaryExprParen::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

WC_END_NAMESPACE
