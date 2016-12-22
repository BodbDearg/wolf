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
CmpExprTwoOps::CmpExprTwoOps(AddExpr & leftExpr,
                             CmpExpr & rightExpr,
                             DTCodegenBinaryOpFunc codegenBinaryOpFunc,
                             DTCodegenConstBinaryOpFunc codegenConstBinaryOpFunc)
:
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr),
    mCodegenBinaryOpFunc(codegenBinaryOpFunc),
    mCodegenConstBinaryOpFunc(codegenConstBinaryOpFunc)
{
    WC_ASSERT(mCodegenBinaryOpFunc);
    WC_ASSERT(mCodegenConstBinaryOpFunc);
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
    
    // Perform the op:
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return (leftTy.*mCodegenBinaryOpFunc)(cgCtx, *this, *leftVal, rightTy, *rightVal);
}

llvm::Constant * CmpExprTwoOps::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate left and right expressions
    llvm::Constant * leftVal = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftVal, nullptr);
    llvm::Constant * rightVal = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightVal, nullptr);
    
    // Perform the op:
    DataType & leftTy = mLeftExpr.dataType();
    DataType & rightTy = mRightExpr.dataType();
    return (leftTy.*mCodegenConstBinaryOpFunc)(*this, *leftVal, rightTy, *rightVal);
}

//-----------------------------------------------------------------------------
// CmpExprEQ
//-----------------------------------------------------------------------------
CmpExprEQ::CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpEQOp, &DataType::codegenConstCmpEQOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CmpExprNE
//-----------------------------------------------------------------------------
CmpExprNE::CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpNEOp, &DataType::codegenConstCmpNEOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CmpExprLT
//-----------------------------------------------------------------------------
CmpExprLT::CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpLTOp, &DataType::codegenConstCmpLTOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CmpExprLE
//-----------------------------------------------------------------------------
CmpExprLE::CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpLEOp, &DataType::codegenConstCmpLEOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CmpExprGT
//-----------------------------------------------------------------------------
CmpExprGT::CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpGTOp, &DataType::codegenConstCmpGTOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CmpExprGE
//-----------------------------------------------------------------------------
CmpExprGE::CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr, &DataType::codegenCmpGEOp, &DataType::codegenConstCmpGEOp)
{
    WC_EMPTY_FUNC_BODY();
}

WC_END_NAMESPACE
