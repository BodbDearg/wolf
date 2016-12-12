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
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * CmpExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of relational operation result!");
    return nullptr;
}

bool CmpExprTwoOps::compileCheckBothExprsAreInt() const {
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
// CmpExprEQ
//-----------------------------------------------------------------------------

CmpExprEQ::CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprEQ::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpEQ(left, right, "CmpExprEQ_CmpOp");
}

llvm::Constant * CmpExprEQ::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, left, right);
}

//-----------------------------------------------------------------------------
// CmpExprNE
//-----------------------------------------------------------------------------

CmpExprNE::CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprNE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpNE(left, right, "CmpExprNEQ_CmpOp");
}

llvm::Constant * CmpExprNE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, left, right);
}

//-----------------------------------------------------------------------------
// CmpExprLT
//-----------------------------------------------------------------------------

CmpExprLT::CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprLT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLT(left, right, "CmpExprLT_CmpOp");
}

llvm::Constant * CmpExprLT::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLT, left, right);
}

//-----------------------------------------------------------------------------
// CmpExprLE
//-----------------------------------------------------------------------------

CmpExprLE::CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprLE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLE(left, right, "CmpExprLE_CmpOp");
}

llvm::Constant * CmpExprLE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLE, left, right);
}

//-----------------------------------------------------------------------------
// CmpExprGT
//-----------------------------------------------------------------------------

CmpExprGT::CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprGT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGT(left, right, "CmpExprGT_CmpOp");
}

llvm::Constant * CmpExprGT::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGT, left, right);
}

//-----------------------------------------------------------------------------
// CmpExprGE
//-----------------------------------------------------------------------------

CmpExprGE::CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr) : CmpExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CmpExprGE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGE(left, right, "CmpExprGE_CmpOp");
}

llvm::Constant * CmpExprGE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGE, left, right);
}

WC_END_NAMESPACE