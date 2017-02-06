#include "CastExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "PrimaryExpr.hpp"
#include "Type.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CastExpr
//-----------------------------------------------------------------------------
bool CastExpr::peek(const Token * currentToken) {
    return PrimaryExpr::peek(currentToken) || currentToken->type == TokenType::kCast;
}

CastExpr * CastExpr::parse(ParseCtx & parseCtx) {
    // See if 'cast' follows:
    if (parseCtx.tok()->type == TokenType::kCast) {
        // Casting expression: skip 'cast'
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect opening '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'cast' for 'cast()' operator!");
            return nullptr;
        }
        
        // Skip '('
        parseCtx.nextTok();
        
        // Parse the initial assign expression:
        AssignExpr * expr = AssignExpr::parse(parseCtx);
        WC_GUARD(expr, nullptr);
        
        // Expect keyword 'to':
        if (parseCtx.tok()->type != TokenType::kTo) {
            parseCtx.error("Expecting keyword 'to' following the expression to cast inside 'cast()' operator!");
            return nullptr;
        }
        
        // Skip 'to'
        parseCtx.nextTok();
        
        // Parse the type to cast to:
        Type * type = Type::parse(parseCtx);
        WC_GUARD(type, nullptr);
        
        // Expect closing ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expecting closing ')' for 'cast()' operator!");
            return nullptr;
        }
        
        // Skip ')'
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Return the parsed node:
        return WC_NEW_AST_NODE(parseCtx,
                               CastExprCast,
                               *startToken,
                               *expr,
                               *type,
                               *endToken);
    }
    
    // Normal expression that does no cast:
    PrimaryExpr * primaryExpr = PrimaryExpr::parse(parseCtx);
    WC_GUARD(primaryExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, CastExprNoCast, *primaryExpr);
}

//-----------------------------------------------------------------------------
// CastExprNoCast
//-----------------------------------------------------------------------------
CastExprNoCast::CastExprNoCast(PrimaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void CastExprNoCast::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & CastExprNoCast::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CastExprNoCast::getEndToken() const {
    return mExpr.getEndToken();
}

bool CastExprNoCast::isLValue() const {
    return mExpr.isLValue();
}

bool CastExprNoCast::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & CastExprNoCast::dataType() const {
    return mExpr.dataType();
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

void CastExprCast::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & CastExprCast::getStartToken() const {
    return mStartToken;
}

const Token & CastExprCast::getEndToken() const {
    return mEndToken;
}

bool CastExprCast::isLValue() const {
    return false;
}

bool CastExprCast::isConstExpr() const {
    // TODO: allow some casting at compile time in future
    return false;
}

const DataType & CastExprCast::dataType() const {
    return mType.dataType();
}

#warning FIXME - Codegen
#if 0
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
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
