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
            BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
            
            if (!binaryExpr) {
                return nullptr;
            }
            
            // Now construct the full expression
            switch (opToken->type){
                case TokenType::kPlus:
                    return new BinaryExprAdd(*unaryExpr, *binaryExpr);
                case TokenType::kMinus:
                    return new BinaryExprSub(*unaryExpr, *binaryExpr);
                case TokenType::kAsterisk:
                    return new BinaryExprMul(*unaryExpr, *binaryExpr);
                case TokenType::kSlash:
                    return new BinaryExprDiv(*unaryExpr, *binaryExpr);
                    
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
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprUnary::generateCode(llvm::IRBuilder<> & irBuilder) {
    return mExpr.generateCode(irBuilder);
}

BinaryExprTwoOps::BinaryExprTwoOps(UnaryExpr & leftExpr,
                                   BinaryExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

BinaryExprAdd::BinaryExprAdd(UnaryExpr & leftExpr,
                             BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprAdd::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateAdd(left, right);
}

BinaryExprSub::BinaryExprSub(UnaryExpr & leftExpr,
                             BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprSub::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateSub(left, right);
}

BinaryExprMul::BinaryExprMul(UnaryExpr & leftExpr,
                             BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprMul::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateMul(left, right);
}

BinaryExprDiv::BinaryExprDiv(UnaryExpr & leftExpr,
                             BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprDiv::generateCode(llvm::IRBuilder<> & irBuilder) {
    llvm::Value * left = mLeftExpr.generateCode(irBuilder);
    llvm::Value * right = mRightExpr.generateCode(irBuilder);
    return irBuilder.CreateSDiv(left, right);
}

WC_END_NAMESPACE
