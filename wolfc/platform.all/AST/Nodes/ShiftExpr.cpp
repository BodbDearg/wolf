//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ShiftExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "PrefixExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// ShiftExpr
//-----------------------------------------------------------------------------
bool ShiftExpr::peek(const Token * tokenPtr) {
    return PrefixExpr::peek(tokenPtr);
}

ShiftExpr * ShiftExpr::parse(ParseCtx & parseCtx) {
    PrefixExpr * leftExpr = PrefixExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            /* Consume the operator token */\
            parseCtx.nextTok();\
            /* Skip any newlines that follow */\
            parseCtx.skipNewlines();\
            /* Parse the right side of the operator */\
            ShiftExpr * rightExpr = ShiftExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.tok()->type) {
        PARSE_OP(TokenType::kLShift, ShiftExprLShift)       // <<
        PARSE_OP(TokenType::kARShift, ShiftExprARShift)     // >>
        PARSE_OP(TokenType::kLRShift, ShiftExprLRShift)     // >>>
            
        default:
            break;
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, ShiftExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// ShiftExprNoOp
//-----------------------------------------------------------------------------
ShiftExprNoOp::ShiftExprNoOp(PrefixExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void ShiftExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ShiftExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ShiftExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// ShiftExprTwoOps
//-----------------------------------------------------------------------------
ShiftExprTwoOps::ShiftExprTwoOps(PrefixExpr & leftExpr, ShiftExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & ShiftExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & ShiftExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// ShiftExprLShift
//-----------------------------------------------------------------------------
ShiftExprLShift::ShiftExprLShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprARShift
//-----------------------------------------------------------------------------
ShiftExprARShift::ShiftExprARShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprARShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// ShiftExprLRShift
//-----------------------------------------------------------------------------
ShiftExprLRShift::ShiftExprLRShift(PrefixExpr & leftExpr, ShiftExpr & rightExpr) :
    ShiftExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ShiftExprLRShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
