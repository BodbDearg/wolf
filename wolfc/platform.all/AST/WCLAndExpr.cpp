#include "WCLAndExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCNotExpr.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LAndExpr
//-----------------------------------------------------------------------------
bool LAndExpr::peek(const Token * tokenPtr) {
    return NotExpr::peek(tokenPtr);
}

LAndExpr * LAndExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    NotExpr * leftExpr = NotExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is an 'and' for logical and
    if (parseCtx.curTok->type == TokenType::kAnd) {
        // And expression with and. Skip the 'and'
        parseCtx.nextTok();
        
        // Parse the following and expression and create the AST node
        LAndExpr * rightExpr = LAndExpr::parse(parseCtx);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(parseCtx, LAndExprAnd, *leftExpr, *rightExpr);
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(parseCtx, LAndExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// LAndExprNoOp
//-----------------------------------------------------------------------------
LAndExprNoOp::LAndExprNoOp(NotExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & LAndExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & LAndExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool LAndExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool LAndExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & LAndExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * LAndExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * LAndExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * LAndExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// LAndExprAnd
//-----------------------------------------------------------------------------
LAndExprAnd::LAndExprAnd(NotExpr & leftExpr, LAndExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & LAndExprAnd::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & LAndExprAnd::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool LAndExprAnd::isLValue() {
    return false;
}

bool LAndExprAnd::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & LAndExprAnd::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

llvm::Value * LAndExprAnd::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'and' operator result!");
    return nullptr;
}

llvm::Value * LAndExprAnd::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions first:
    llvm::Value * leftValue = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Verify data types used by operator:
    WC_GUARD(compileCheckBothExprsAreBool(), nullptr);

    // Create the logical operation:
    return cgCtx.irBuilder.CreateAnd(rightValue, leftValue, "LAndExprAnd_AndOp");
}

llvm::Constant * LAndExprAnd::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions first:
    llvm::Constant * leftValue = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Constant * rightValue = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Verify data types used by operator:
    WC_GUARD(compileCheckBothExprsAreBool(), nullptr);
    
    // Create the logical operation:
    return llvm::ConstantExpr::getAnd(rightValue, leftValue);
}

bool LAndExprAnd::compileCheckBothExprsAreBool() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.isBool()) {
        compileError("Left side of logical 'and' expression must evaluate to 'bool', not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.isBool()) {
        compileError("Right side of logical 'and' expression must evaluate to 'bool', not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
