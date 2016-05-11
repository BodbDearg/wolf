#include "WCBinaryExpr.hpp"
#include "WCUnaryExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool BinaryExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

BinaryExpr * BinaryExpr::parse(const Token *& tokenPtr) {
    UnaryExpr * unaryExpr = UnaryExpr::parse(tokenPtr);
    
    if (!unaryExpr) {
        return nullptr;
    }
    
    switch (tokenPtr->type) {
        // Two operand binary expression
        case TokenType::kPlus:
        case TokenType::kMinus:
        case TokenType::kAsterisk:
        case TokenType::kSlash:
        {
            // Parse the right expression
            const Token * opToken = tokenPtr;
            ++tokenPtr;
            UnaryExpr * unaryExpr2 = UnaryExpr::parse(tokenPtr);
            
            if (!unaryExpr) {
                return nullptr;
            }
            
            // Now construct the full expression
            switch (opToken->type){
                case TokenType::kPlus:
                    return new BinaryExprAdd(*unaryExpr, *unaryExpr2);
                case TokenType::kMinus:
                    return new BinaryExprSub(*unaryExpr, *unaryExpr2);
                case TokenType::kAsterisk:
                    return new BinaryExprMul(*unaryExpr, *unaryExpr2);
                case TokenType::kSlash:
                    return new BinaryExprDiv(*unaryExpr, *unaryExpr2);
                    
                default:
                    WC_ASSERT(0);
                    break;
            }
            
        }   break;
            
        // No-op binary expression
        default:
            return new BinaryExprUnary(*unaryExpr);
    }
    
    return nullptr;
}

BinaryExprUnary::BinaryExprUnary(UnaryExpr & expr) :
    mExpr(expr)
{
    
}

llvm::Value * BinaryExprUnary::generateCode(llvm::IRBuilder<> & irBuilder) {
    return mExpr.generateCode(irBuilder);
}

BinaryExprTwoOps::BinaryExprTwoOps(UnaryExpr & leftExpr,
                                   UnaryExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    // Nothing to do here...
}

BinaryExprAdd::BinaryExprAdd(UnaryExpr & leftExpr,
                             UnaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    // Nothing to do here...
}

llvm::Value * BinaryExprAdd::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateAdd(left, right);
}

BinaryExprSub::BinaryExprSub(UnaryExpr & leftExpr,
                             UnaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    // Nothing to do here...
}

llvm::Value * BinaryExprSub::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateSub(left, right);
}

BinaryExprMul::BinaryExprMul(UnaryExpr & leftExpr,
                             UnaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    // Nothing to do here...
}

llvm::Value * BinaryExprMul::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateMul(left, right);
}

BinaryExprDiv::BinaryExprDiv(UnaryExpr & leftExpr,
                             UnaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    // Nothing to do here...
}

llvm::Value * BinaryExprDiv::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateSDiv(left, right);
}

WC_END_NAMESPACE
