#include "WCLOrExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLAndExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// LOrExpr
//-----------------------------------------------------------------------------
bool LOrExpr::peek(const Token * tokenPtr) {
    return LAndExpr::peek(tokenPtr);
}

LOrExpr * LOrExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    LAndExpr * andExpr = LAndExpr::parse(parseCtx);
    WC_GUARD(andExpr, nullptr);
    
    // See if there is an 'or' for logical or:
    if (parseCtx.curTok->type == TokenType::kOr) {
        TokenType nextTokType = parseCtx.curTok[1].type;

        // Note: do not parse if we find 'or if' or 'or unless' since those are block
        // terminators for a 'if / or if / else' chain
        if (nextTokType != TokenType::kIf &&
            nextTokType != TokenType::kUnless)
        {
            // Or expression with or. Skip the 'or'
            parseCtx.nextTok();
        
            // Parse the following and expression and create the AST node
            LOrExpr * orExpr = LOrExpr::parse(parseCtx);
            WC_GUARD(orExpr, nullptr);
            return WC_NEW_AST_NODE(parseCtx, LOrExprOr, *andExpr, *orExpr);
        }
    }

    // Basic no-op expression
    return WC_NEW_AST_NODE(parseCtx, LOrExprNoOp, *andExpr);
}

//-----------------------------------------------------------------------------
// LOrExprNoOp
//-----------------------------------------------------------------------------
LOrExprNoOp::LOrExprNoOp(LAndExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & LOrExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & LOrExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool LOrExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool LOrExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & LOrExprNoOp::dataType() {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * LOrExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * LOrExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * LOrExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// LOrExprOr
//-----------------------------------------------------------------------------
LOrExprOr::LOrExprOr(LAndExpr & leftExpr, LOrExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & LOrExprOr::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & LOrExprOr::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool LOrExprOr::isLValue() {
    return false;
}

bool LOrExprOr::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & LOrExprOr::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

#warning FIXME - Codegen
#if 0
llvm::Value * LOrExprOr::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'or' operator result!");
    return nullptr;
}

llvm::Value * LOrExprOr::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions first:
    llvm::Value * leftValue = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Verify data types used by operator:
    WC_GUARD(compileCheckBothExprsAreBool(), nullptr);
    
    // Create the logical operation:
    return cgCtx.irBuilder.CreateOr(rightValue, leftValue, "LOrExprOr_OrOp");
}

llvm::Constant * LOrExprOr::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: add support for lazy evaluation
    // Evaluate the sub expressions first:
    llvm::Constant * leftValue = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(leftValue, nullptr);
    llvm::Constant * rightValue = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Verify data types used by operator:
    WC_GUARD(compileCheckBothExprsAreBool(), nullptr);
    
    // Create the logical operation:
    return llvm::ConstantExpr::getOr(rightValue, leftValue);
}

bool LOrExprOr::compileCheckBothExprsAreBool() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.isBool()) {
        compileError("Left side of logical 'or' expression must evaluate to 'bool', not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.isBool()) {
        compileError("Right side of logical 'or' expression must evaluate to 'bool', not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
