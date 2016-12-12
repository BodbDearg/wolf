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
        PARSE_OP(TokenType::kAsterisk, MulExprMul)      // *
        PARSE_OP(TokenType::kSlash, MulExprDiv)         // /
        PARSE_OP(TokenType::kPercent, MulExprMod)       // %
        PARSE_OP(TokenType::kAmpersand, MulExprBAnd)    // &
            
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

llvm::Value * MulExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Value * leftVal = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Value * rightVal = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Codegen the op itself
    return codegenOpEval(cgCtx, *leftVal, *rightVal);
}

llvm::Constant * MulExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Constant * leftVal = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Constant * rightVal = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Codegen the op itself
    return codegenOpConstEval(*leftVal, *rightVal);
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

llvm::Value * MulExprMul::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateMul(&leftVal, &rightVal, "MulExprMul_MulOp");
}

llvm::Constant * MulExprMul::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getMul(&leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// MulExprDiv
//-----------------------------------------------------------------------------

MulExprDiv::MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprDiv::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateSDiv(&leftVal, &rightVal, "MulExprDiv_DivOp");
}

llvm::Constant * MulExprDiv::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getSDiv(&leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// MulExprMod
//-----------------------------------------------------------------------------

MulExprMod::MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprMod::codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateSRem(&leftVal, &rightVal, "MulExprMod_ModOp");
}

llvm::Constant * MulExprMod::codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getSRem(&leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// MulExprBAnd
//-----------------------------------------------------------------------------

MulExprBAnd::MulExprBAnd(ShiftExpr & leftExpr, MulExpr & rightExpr) :
    MulExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * MulExprBAnd::codegenOpEval(CodegenCtx & cgCtx,
                                         llvm::Value & leftVal,
                                         llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateAnd(&leftVal, &rightVal, "MulExprBAnd_AndOp");
}

llvm::Constant * MulExprBAnd::codegenOpConstEval(llvm::Constant & leftVal,
                                                 llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getAnd(&leftVal, &rightVal);
}

WC_END_NAMESPACE
