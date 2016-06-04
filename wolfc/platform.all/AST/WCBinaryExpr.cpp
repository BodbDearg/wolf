#include "WCBinaryExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"
#include "WCUnaryExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// BinaryExpr
//-----------------------------------------------------------------------------

bool BinaryExpr::peek(const Token * tokenPtr) {
    return UnaryExpr::peek(tokenPtr);
}

BinaryExpr * BinaryExpr::parse(const Token *& tokenPtr) {
    UnaryExpr * unaryExpr = UnaryExpr::parse(tokenPtr);
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
            BinaryExpr * binaryExpr = BinaryExpr::parse(tokenPtr);
            WC_GUARD(binaryExpr, nullptr);
            
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

//-----------------------------------------------------------------------------
// BinaryExprUnary
//-----------------------------------------------------------------------------

BinaryExprUnary::BinaryExprUnary(UnaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & BinaryExprUnary::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & BinaryExprUnary::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * BinaryExprUnary::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool BinaryExprUnary::isLValue() const {
    return mExpr.isLValue();
}

const DataType & BinaryExprUnary::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * BinaryExprUnary::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// BinaryExprTwoOps
//-----------------------------------------------------------------------------

BinaryExprTwoOps::BinaryExprTwoOps(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & BinaryExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & BinaryExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool BinaryExprTwoOps::isLValue() const {
    return false;
}

const DataType & BinaryExprTwoOps::getDataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.getDataType();
}

llvm::Value * BinaryExprTwoOps::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

bool BinaryExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.getDataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in binary expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.getDataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in binary expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// BinaryExprAdd
//-----------------------------------------------------------------------------

BinaryExprAdd::BinaryExprAdd(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprAdd::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateAdd(left, right, "bin_expr_add");
}

//-----------------------------------------------------------------------------
// BinaryExprSub
//-----------------------------------------------------------------------------

BinaryExprSub::BinaryExprSub(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprSub::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSub(left, right, "bin_expr_sub");
}

//-----------------------------------------------------------------------------
// BinaryExprMul
//-----------------------------------------------------------------------------

BinaryExprMul::BinaryExprMul(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprMul::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateMul(left, right, "bin_expr_mul");
}

//-----------------------------------------------------------------------------
// BinaryExprDiv
//-----------------------------------------------------------------------------

BinaryExprDiv::BinaryExprDiv(UnaryExpr & leftExpr, BinaryExpr & rightExpr) :
    BinaryExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * BinaryExprDiv::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSDiv(left, right, "bin_expr_div");
}

WC_END_NAMESPACE
