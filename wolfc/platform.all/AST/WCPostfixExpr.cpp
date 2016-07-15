#include "WCPostfixExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PostfixExpr
//-----------------------------------------------------------------------------

bool PostfixExpr::peek(const Token * currentToken) {
    return PrimaryExpr::peek(currentToken);
}

PostfixExpr * PostfixExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    PrimaryExpr * expr = PrimaryExpr::parse(currentToken, alloc);
    WC_GUARD(expr, nullptr);
    
    // TODO: the rest
    
    // No postfix:
    return WC_NEW_AST_NODE(alloc, PostfixExprNoPostfix, *expr);
}

//-----------------------------------------------------------------------------
// PostfixExprNoPostfix
//-----------------------------------------------------------------------------

PostfixExprNoPostfix::PostfixExprNoPostfix(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & PostfixExprNoPostfix::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprNoPostfix::getEndToken() const {
    return mExpr.getEndToken();
}

bool PostfixExprNoPostfix::isLValue() const {
    return mExpr.isLValue();
}

const DataType & PostfixExprNoPostfix::dataType() const {
    return mExpr.dataType();
}

llvm::Value * PostfixExprNoPostfix::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PostfixExprNoPostfix::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprNegPrimary
//-----------------------------------------------------------------------------
/*
UnaryExprNegPrimary::UnaryExprNegPrimary(PrimaryExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & UnaryExprNegPrimary::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprNegPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprNegPrimary::isLValue() const {
    return false;
}

const DataType & UnaryExprNegPrimary::dataType() const {
    return mExpr.dataType();
}

llvm::Value * UnaryExprNegPrimary::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprNegPrimary::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: support more types
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Unary '-' operator only supports 'int' datatype, not '%s'!", exprType.name());
        return nullptr;
    }
    
    return cgCtx.irBuilder.CreateNeg(mExpr.codegenExprEval(cgCtx));
}
*/

WC_END_NAMESPACE
