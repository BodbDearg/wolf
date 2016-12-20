#include "WCCmpExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAddExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CmpExpr
//-----------------------------------------------------------------------------

bool CmpExpr::peek(const Token * tokenPtr) {
    return AddExpr::peek(tokenPtr);
}

CmpExpr * CmpExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    AddExpr * leftExpr = AddExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            ++tokenPtr;\
            CmpExpr * rightExpr = CmpExpr::parse(tokenPtr, alloc);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(alloc, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (tokenPtr[0].type) {
        PARSE_OP(TokenType::kCmpEQ, CmpExprEQ)  // ==
        PARSE_OP(TokenType::kCmpNE, CmpExprNE)  // !=
        PARSE_OP(TokenType::kCmpLT, CmpExprLT)  // <
        PARSE_OP(TokenType::kCmpLE, CmpExprLE)  // <=
        PARSE_OP(TokenType::kCmpGT, CmpExprGT)  // >
        PARSE_OP(TokenType::kCmpGE, CmpExprGE)  // >=
            
        default:
            break;
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, CmpExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// CmpExprNoOp
//-----------------------------------------------------------------------------

CmpExprNoOp::CmpExprNoOp(AddExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & CmpExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CmpExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool CmpExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool CmpExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & CmpExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * CmpExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * CmpExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * CmpExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// CmpExprTwoOps
//-----------------------------------------------------------------------------

CmpExprTwoOps::CmpExprTwoOps(AddExpr & leftExpr, CmpExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & CmpExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & CmpExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool CmpExprTwoOps::isLValue() {
    return false;
}

bool CmpExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & CmpExprTwoOps::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

llvm::Value * CmpExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of relational operation result!");
    return nullptr;
}

llvm::Value * CmpExprTwoOps::codegenExprEval(CodegenCtx & cgCtx) {
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

llvm::Constant * CmpExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
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

bool CmpExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// CmpExprEQ
//-----------------------------------------------------------------------------

CmpExprEQ::CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprEQ::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpEQ(&leftVal, &rightVal, "CmpExprEQ_CmpOp");
}

llvm::Constant * CmpExprEQ::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, &leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprNE
//-----------------------------------------------------------------------------

CmpExprNE::CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprNE::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpNE(&leftVal, &rightVal, "CmpExprNE_CmpOp");
}

llvm::Constant * CmpExprNE::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, &leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprLT
//-----------------------------------------------------------------------------

CmpExprLT::CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprLT::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpSLT(&leftVal, &rightVal, "CmpExprLT_CmpOp");
}

llvm::Constant * CmpExprLT::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLT, &leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprLE
//-----------------------------------------------------------------------------

CmpExprLE::CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprLE::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpSLE(&leftVal, &rightVal, "CmpExprLE_CmpOp");
}

llvm::Constant * CmpExprLE::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLE, &leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprGT
//-----------------------------------------------------------------------------

CmpExprGT::CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprGT::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpSGT(&leftVal, &rightVal, "CmpExprGT_CmpOp");
}

llvm::Constant * CmpExprGT::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGT, &leftVal, &rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprGE
//-----------------------------------------------------------------------------

CmpExprGE::CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprGE::codegenOpEval(CodegenCtx & cgCtx,
                                       llvm::Value & leftVal,
                                       llvm::Value & rightVal)
{
    return cgCtx.irBuilder.CreateICmpSGE(&leftVal, &rightVal, "CmpExprGE_CmpOp");
}

llvm::Constant * CmpExprGE::codegenOpConstEval(llvm::Constant & leftVal,
                                               llvm::Constant & rightVal)
{
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGE, &leftVal, &rightVal);
}

WC_END_NAMESPACE
