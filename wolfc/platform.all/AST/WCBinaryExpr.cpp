#include "WCBinaryExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"
#include "WCUnaryExpr.hpp"

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

llvm::Value * BinaryExprUnary::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

BinaryExprTwoOps::BinaryExprTwoOps(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

BinaryExprAdd::BinaryExprAdd(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprAdd::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateAdd(left, right);
}

BinaryExprSub::BinaryExprSub(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprSub::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateSub(left, right);
}

BinaryExprMul::BinaryExprMul(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprMul::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateMul(left, right);
}

BinaryExprDiv::BinaryExprDiv(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY()
}

llvm::Value * BinaryExprDiv::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateSDiv(left, right);
}

WC_END_NAMESPACE
