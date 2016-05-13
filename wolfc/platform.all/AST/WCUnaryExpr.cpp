#include "WCUnaryExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIntLit.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool UnaryExpr::peek(const Token * currentToken) {
    if (currentToken->type == TokenType::kIntLit ||
        currentToken->type == TokenType::kMinus ||
        currentToken->type == TokenType::kPlus ||
        currentToken->type == TokenType::kLParen)
    {
        return true;
    }
    
    return false;
}

UnaryExpr * UnaryExpr::parse(const Token *& currentToken) {
    switch (currentToken->type) {
        /* UIntLit */
        case TokenType::kIntLit: {
            IntLit * uintLit = IntLit::parse(currentToken);
            WC_GUARD(uintLit, nullptr);
            return new UnaryExprIntLit(*uintLit);
        }   break;
            
        /* -IntLit */
        case TokenType::kMinus: {
            ++currentToken; // Skip '-'
            IntLit * uintLit = IntLit::parse(currentToken);
            WC_GUARD(uintLit, nullptr);
            return new UnaryExprIntLitNeg(*uintLit);
        }   break;
            
        /* +IntLit */
        case TokenType::kPlus: {
            ++currentToken; // Skip '+'
            IntLit * uintLit = IntLit::parse(currentToken);
            WC_GUARD(uintLit, nullptr);
            return new UnaryExprIntLitPos(*uintLit);
        }   break;
            
        /* (BinaryExpr) */
        case TokenType::kLParen: {
            const Token * lparenToken = currentToken;
            ++currentToken; // Skip '('
            
            BinaryExpr * expr = BinaryExpr::parse(currentToken);
            WC_GUARD(expr, nullptr);
            
            if (currentToken->type != TokenType::kRParen) {
                error(*currentToken,
                      "Expected closing ')' to match '(' at line %zu and column %zu!",
                      lparenToken->srcLine,
                      lparenToken->srcCol);
                
                return nullptr;
            }
            
            ++currentToken; // Skip ')'
            return new UnaryExprParen(*expr);
        }   break;
            
        default:
            break;
    }
    
    error(*currentToken, "Expected Unary Expression!");
    return nullptr;
}

UnaryExprIntLit::UnaryExprIntLit(IntLit & lit) : mLit(lit) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * UnaryExprIntLit::generateCode(const CodegenCtx & cgCtx) {
    return mLit.generateCode(cgCtx);
}

UnaryExprIntLitNeg::UnaryExprIntLitNeg(IntLit & lit) : mLit(lit) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * UnaryExprIntLitNeg::generateCode(const CodegenCtx & cgCtx) {
    return cgCtx.irBuilder.CreateNeg(mLit.generateCode(cgCtx));
}

UnaryExprIntLitPos::UnaryExprIntLitPos(IntLit & lit) : UnaryExprIntLit(lit) {
    WC_EMPTY_FUNC_BODY();
}

UnaryExprParen::UnaryExprParen(BinaryExpr & expr) : mExpr(expr) {
    WC_EMPTY_FUNC_BODY();
}
    
llvm::Value * UnaryExprParen::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

WC_END_NAMESPACE
