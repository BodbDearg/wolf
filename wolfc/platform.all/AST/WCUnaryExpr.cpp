#include "WCUnaryExpr.hpp"
#include "WCBinaryExpr.hpp"
#include "WCIntLit.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool UnaryExpr::peek(const Token * currentToken) {
    if (currentToken->type == TokenType::kIntLiteral ||
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
        case TokenType::kIntLiteral: {
            IntLit * uintLit = IntLit::parse(currentToken);
            
            if (!uintLit) {
                return nullptr;
            }
            
            return new UnaryExprIntLit(*uintLit);
        }   break;
            
        /* -IntLit */
        case TokenType::kMinus: {
            ++currentToken; // Skip '-'
            IntLit * uintLit = IntLit::parse(currentToken);
            
            if (!uintLit) {
                return nullptr;
            }
            
            return new UnaryExprIntLitNeg(*uintLit);
        }   break;
            
        /* +IntLit */
        case TokenType::kPlus: {
            ++currentToken; // Skip '+'
            IntLit * uintLit = IntLit::parse(currentToken);
            
            if (!uintLit) {
                return nullptr;
            }
            
            return new UnaryExprIntLitPos(*uintLit);
        }   break;
            
        /* (BinaryExpr) */
        case TokenType::kLParen: {
            const Token * lparenToken = currentToken;
            ++currentToken; // Skip '('
            BinaryExpr * expr = BinaryExpr::parse(currentToken);
            
            if (!expr) {
                return nullptr;
            }
            
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
    // Nothing to do here...
}

llvm::Value * UnaryExprIntLit::generateCode(llvm::IRBuilder<> & irBuilder) {
    return mLit.generateCode(irBuilder);
}

UnaryExprIntLitNeg::UnaryExprIntLitNeg(IntLit & lit) : mLit(lit) {
    // Nothing to do here...
}

llvm::Value * UnaryExprIntLitNeg::generateCode(llvm::IRBuilder<> & irBuilder) {
    return irBuilder.CreateNeg(mLit.generateCode(irBuilder));
}

UnaryExprIntLitPos::UnaryExprIntLitPos(IntLit & lit) : UnaryExprIntLit(lit) {
    // Nothing to do here...
}

UnaryExprParen::UnaryExprParen(BinaryExpr & expr) : mExpr(expr) {
    // Nothing to do here...
}
    
llvm::Value * UnaryExprParen::generateCode(llvm::IRBuilder<> & irBuilder) {
    return mExpr.generateCode(irBuilder);
}

WC_END_NAMESPACE
