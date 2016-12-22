#include "WCCastExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CastExpr
//-----------------------------------------------------------------------------
bool CastExpr::peek(const Token * currentToken) {
    return PrimaryExpr::peek(currentToken) || currentToken->type == TokenType::kCast;
}

CastExpr * CastExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // See if 'cast' follows:
    if (currentToken->type == TokenType::kCast) {
        // Casting expression: skip 'cast'
        const Token * startToken = currentToken;
        ++currentToken;
        
        // Expect opening '('
        if (currentToken->type != TokenType::kLParen) {
            parseError(*currentToken, "Expect '(' following 'cast' for 'cast()' operator!");
            return nullptr;
        }
        
        // Skip '('
        ++currentToken;
        
        // Parse the initial assign expression:
        AssignExpr * expr = AssignExpr::parse(currentToken, alloc);
        WC_GUARD(expr, nullptr);
        
        // Expect keyword 'to':
        if (currentToken->type != TokenType::kTo) {
            parseError(*currentToken,
                       "Expecting keyword 'to' following the expression to cast inside 'cast()' operator!");
            
            return nullptr;
        }
        
        // Skip 'to'
        ++currentToken;
        
        // Parse the type to cast to:
        Type * type = Type::parse(currentToken, alloc);
        WC_GUARD(type, nullptr);
        
        // Expect closing ')'
        if (currentToken->type != TokenType::kRParen) {
            parseError(*currentToken,
                       "Expecting closing ')' for 'cast()' operator!");
            
            return nullptr;
        }
        
        // Skip ')'
        const Token * endToken = currentToken;
        ++currentToken;
        
        // Return the parsed node:
        return WC_NEW_AST_NODE(alloc,
                               CastExprCast,
                               *startToken,
                               *expr,
                               *type,
                               *endToken);
    }
    
    // Normal expression that does no cast:
    PrimaryExpr * primaryExpr = PrimaryExpr::parse(currentToken, alloc);
    WC_GUARD(primaryExpr, nullptr);
    return WC_NEW_AST_NODE(alloc, CastExprNoCast, *primaryExpr);
}

//-----------------------------------------------------------------------------
// CastExprNoCast
//-----------------------------------------------------------------------------
CastExprNoCast::CastExprNoCast(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & CastExprNoCast::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CastExprNoCast::getEndToken() const {
    return mExpr.getEndToken();
}

bool CastExprNoCast::isLValue() {
    return mExpr.isLValue();
}

bool CastExprNoCast::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & CastExprNoCast::dataType() {
    return mExpr.dataType();
}

llvm::Value * CastExprNoCast::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * CastExprNoCast::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * CastExprNoCast::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// CastExprCast
//-----------------------------------------------------------------------------
CastExprCast::CastExprCast(const Token & startToken,
                           AssignExpr & expr,
                           Type & type,
                           const Token & endToken)
:
    mStartToken(startToken),
    mExpr(expr),
    mType(type),
    mEndToken(endToken)
{
    mExpr.mParent = this;
    mType.mParent = this;
}

const Token & CastExprCast::getStartToken() const {
    return mStartToken;
}

const Token & CastExprCast::getEndToken() const {
    return mEndToken;
}

bool CastExprCast::isLValue() {
    return false;
}

bool CastExprCast::isConstExpr() {
    // TODO: allow some casting at compile time in future
    return false;
}

DataType & CastExprCast::dataType() {
    return mType.dataType();
}

llvm::Value * CastExprCast::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cant take the address of a 'cast()' expression!");
    return nullptr;
}

llvm::Value * CastExprCast::codegenExprEval(CodegenCtx & cgCtx) {
    // First evaluate the expression to cast:
    llvm::Value * exprValue = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(exprValue, nullptr);
    
    // Now, do the cast and return the result:
    DataType & exprDataType = mExpr.dataType();
    return exprDataType.codegenCastTo(cgCtx,
                                      *this,
                                      *exprValue,
                                      mType.dataType());
}

llvm::Constant * CastExprCast::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: allow some casting at compile time in future
    WC_UNUSED_PARAM(cgCtx);
    compileError("'cast()' currently cannot be used in constant expressions!");
    return nullptr;
}

WC_END_NAMESPACE
