#include "WCMulExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCShiftExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// MulExpr
//-----------------------------------------------------------------------------

bool MulExpr::peek(const Token * tokenPtr) {
    return ShiftExpr::peek(tokenPtr);
}

MulExpr * MulExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    ShiftExpr * leftExpr = ShiftExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            ++tokenPtr;\
            MulExpr * rightExpr = MulExpr::parse(tokenPtr, alloc);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(alloc, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (tokenPtr[0].type) {
        PARSE_OP(TokenType::kAsterisk, MulExprMul)  // *
        PARSE_OP(TokenType::kSlash, MulExprDiv)     // /
        PARSE_OP(TokenType::kPercent, MulExprMod)   // %
            
        default:
            break;
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, MulExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// MulExprNoOp
//-----------------------------------------------------------------------------

MulExprNoOp::MulExprNoOp(ShiftExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & MulExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & MulExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool MulExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool MulExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & MulExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * MulExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * MulExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * MulExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// MulExprTwoOps
//-----------------------------------------------------------------------------

MulExprTwoOps::MulExprTwoOps(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & MulExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & MulExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool MulExprTwoOps::isLValue() {
    return false;
}

bool MulExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & MulExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * MulExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of '*' or '/' operator result!");
    return nullptr;
}

bool MulExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// MulExprMul
//-----------------------------------------------------------------------------

MulExprMul::MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprMul::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateMul(left, right, "MulExprMul_MulOp");
}

llvm::Constant * MulExprMul::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getMul(left, right);
}

//-----------------------------------------------------------------------------
// MulExprDiv
//-----------------------------------------------------------------------------

MulExprDiv::MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprDiv::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSDiv(left, right, "MulExprDiv_DivOp");
}

llvm::Constant * MulExprDiv::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getSDiv(left, right);
}

//-----------------------------------------------------------------------------
// MulExprMod
//-----------------------------------------------------------------------------

MulExprMod::MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprMod::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSRem(left, right, "MulExprMod_ModOp");
}

llvm::Constant * MulExprMod::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);

    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getSRem(left, right);
}

WC_END_NAMESPACE
