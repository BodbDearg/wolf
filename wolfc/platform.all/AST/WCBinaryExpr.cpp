#include "WCBinaryExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCToken.hpp"
#include "WCUnaryExpr.hpp"

WC_BEGIN_NAMESPACE

BinaryExpr::BinaryExpr(ASTNode & parent) : ASTNodeCodegen(parent) {
    WC_EMPTY_FUNC_BODY();
}

bool BinaryExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

BinaryExpr * BinaryExpr::parse(ASTNode & parent, const Token *& tokenPtr) {
    UnaryExpr * unaryExpr = UnaryExpr::parse(parent, tokenPtr);
    WC_GUARD(unaryExpr, nullptr);
    
    switch (tokenPtr->type) {
        // Two operand binary expression
        case TokenType::kPlus:
        case TokenType::kMinus:
        case TokenType::kAsterisk:
        case TokenType::kSlash: {
            // Parse the right expression
            const Token * opToken = tokenPtr;
            ++tokenPtr;
            BinaryExpr * binaryExpr = BinaryExpr::parse(parent, tokenPtr);
            WC_GUARD(binaryExpr, nullptr);
            
            // Now construct the full expression
            switch (opToken->type){
                case TokenType::kPlus:
                    return new BinaryExprAdd(parent, *unaryExpr, *binaryExpr);
                case TokenType::kMinus:
                    return new BinaryExprSub(parent, *unaryExpr, *binaryExpr);
                case TokenType::kAsterisk:
                    return new BinaryExprMul(parent, *unaryExpr, *binaryExpr);
                case TokenType::kSlash:
                    return new BinaryExprDiv(parent, *unaryExpr, *binaryExpr);
                    
                default:
                    WC_ASSERT(0);
                    break;
            }
            
        }   break;
            
        // No-op binary expression
        default:
            return new BinaryExprUnary(parent, *unaryExpr);
    }
    
    return nullptr;
}

BinaryExprUnary::BinaryExprUnary(ASTNode & parent, UnaryExpr & expr) :
    BinaryExpr(parent),
    mExpr(expr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprUnary::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

BinaryExprTwoOps::BinaryExprTwoOps(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExpr(parent),
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

BinaryExprAdd::BinaryExprAdd(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(parent, leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprAdd::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateAdd(left, right);
}

BinaryExprSub::BinaryExprSub(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(parent, leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprSub::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateSub(left, right);
}

BinaryExprMul::BinaryExprMul(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(parent, leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprMul::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateMul(left, right);
}

BinaryExprDiv::BinaryExprDiv(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(parent, leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprDiv::generateCode(const CodegenCtx & cgCtx) {
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    return cgCtx.irBuilder.CreateSDiv(left, right);
}

WC_END_NAMESPACE
