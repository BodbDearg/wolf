#include "WCUnaryExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPostfixExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// UnaryExpr
//-----------------------------------------------------------------------------
bool UnaryExpr::peek(const Token * currentToken) {
    /* 
    -PostfixExpr
    +PostfixExpr
    */
    if (currentToken->type == TokenType::kMinus || currentToken->type == TokenType::kPlus) {
        return PostfixExpr::peek(currentToken + 1);
    }
    
    /* (AssignExpr) */
    if (currentToken->type == TokenType::kLParen) {
        return AssignExpr::peek(currentToken + 1);
    }
    
    /* PrimaryExpr */
    return PostfixExpr::peek(currentToken);
}

UnaryExpr * UnaryExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    switch (currentToken->type) {
        /* -PostfixExpr */
        case TokenType::kMinus: {
            const Token * minusTok = currentToken;
            ++currentToken;     // Skip '-'
            PostfixExpr * expr = PostfixExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprMinus, *expr, *minusTok);
        }   break;
            
        /* +PostfixExpr */
        case TokenType::kPlus: {
            const Token * plusTok = currentToken;
            ++currentToken;     // Skip '+'
            PostfixExpr * expr = PostfixExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprPlus, *expr, *plusTok);
        }   break;
            
        /* (AssignExpr) */
        case TokenType::kLParen: {
            const Token * lparenTok = currentToken;
            ++currentToken;     // Skip '('
            AssignExpr * expr = AssignExpr::parse(currentToken, alloc);
            
            if (currentToken->type != TokenType::kRParen) {
                parseError(*currentToken,
                           "Expected closing ')' to match '(' at line %zu and column %zu!",
                           lparenTok->startLine + 1,
                           lparenTok->startCol + 1);
                
                return nullptr;
            }
            
            const Token * rparenTok = currentToken;
            ++currentToken;     // Skip ')'
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprParen, *expr, *lparenTok, *rparenTok);
        }   break;
            
        /* PostfixExpr */
        default: {
            PostfixExpr * expr = PostfixExpr::parse(currentToken, alloc);
            WC_GUARD(expr, nullptr);
            return WC_NEW_AST_NODE(alloc, UnaryExprPrimary, *expr);
        }   break;
    }
    
    return nullptr;     // Should never reach here
}

//-----------------------------------------------------------------------------
// UnaryExprPrimary
//-----------------------------------------------------------------------------
UnaryExprPrimary::UnaryExprPrimary(PostfixExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & UnaryExprPrimary::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & UnaryExprPrimary::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprPrimary::isLValue() {
    return mExpr.isLValue();
}

bool UnaryExprPrimary::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & UnaryExprPrimary::dataType() {
    return mExpr.dataType();
}

llvm::Value * UnaryExprPrimary::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprPrimary::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * UnaryExprPrimary::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// UnaryExprPlusMinusBase
//-----------------------------------------------------------------------------
UnaryExprPlusMinusBase::UnaryExprPlusMinusBase(PostfixExpr & expr,
                                               const Token & startToken,
                                               DTCodegenUnaryOpFunc codegenUnaryOpFunc,
                                               DTCodegenConstUnaryOpFunc codegenConstUnaryOpFunc)
:
    UnaryExprPrimary(expr),
    mStartToken(startToken),
    mCodegenUnaryOpFunc(codegenUnaryOpFunc),
    mCodegenConstUnaryOpFunc(codegenConstUnaryOpFunc)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & UnaryExprPlusMinusBase::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprPlusMinusBase::getEndToken() const {
    return mExpr.getEndToken();
}

bool UnaryExprPlusMinusBase::isLValue() {
    return false;
}

llvm::Value * UnaryExprPlusMinusBase::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a unary expression result!");
    return nullptr;
}

llvm::Value * UnaryExprPlusMinusBase::codegenExprEval(CodegenCtx & cgCtx) {
    // Generate the code for the expression value first
    llvm::Value * exprValue = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(exprValue, nullptr);
    
    // Now do the operation
    DataType & exprType = mExpr.dataType();
    return (exprType.*mCodegenUnaryOpFunc)(cgCtx, *this, *exprValue);
}

llvm::Constant * UnaryExprPlusMinusBase::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Generate the code for the expression value first
    llvm::Constant * exprValue = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(exprValue, nullptr);
    
    // Now do the operation
    DataType & exprType = mExpr.dataType();
    return (exprType.*mCodegenConstUnaryOpFunc)(*this, *exprValue);
}

//-----------------------------------------------------------------------------
// UnaryExprPlus
//-----------------------------------------------------------------------------
UnaryExprPlus::UnaryExprPlus(PostfixExpr & expr, const Token & startToken) :
    UnaryExprPlusMinusBase(expr, startToken, &DataType::codegenPlusOp, &DataType::codegenConstPlusOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// UnaryExprMinus
//-----------------------------------------------------------------------------
UnaryExprMinus::UnaryExprMinus(PostfixExpr & expr, const Token & startToken) :
    UnaryExprPlusMinusBase(expr, startToken, &DataType::codegenMinusOp, &DataType::codegenConstMinusOp)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// UnaryExprParen
//-----------------------------------------------------------------------------
UnaryExprParen::UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken) :
    mExpr(expr),
    mStartToken(startToken),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

const Token & UnaryExprParen::getStartToken() const {
    return mStartToken;
}

const Token & UnaryExprParen::getEndToken() const {
    return mEndToken;
}

bool UnaryExprParen::isLValue() {
    return mExpr.isLValue();
}

bool UnaryExprParen::isConstExpr() {
    return mExpr.isLValue();
}

DataType & UnaryExprParen::dataType() {
    return mExpr.dataType();
}

llvm::Value * UnaryExprParen::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * UnaryExprParen::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * UnaryExprParen::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

WC_END_NAMESPACE
