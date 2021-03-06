//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "MulExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AddExpr
//-----------------------------------------------------------------------------
bool AddExpr::peek(const Token * tokenPtr) {
    return MulExpr::peek(tokenPtr);
}

AddExpr * AddExpr::parse(ParseCtx & parseCtx) {
    MulExpr * leftExpr = MulExpr::parse(parseCtx);
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
            AddExpr * rightExpr = AddExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.tok()->type) {
        PARSE_OP(TokenType::kPlus, AddExprAdd)      // +
        PARSE_OP(TokenType::kMinus, AddExprSub)     // -
        PARSE_OP(TokenType::kVBar, AddExprBOr)      // |
        PARSE_OP(TokenType::kHat, AddExprBXor)      // ^
            
        default:
            break;
    }
    
    #undef PARSE_OP
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, AddExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// AddExprNoOp
//-----------------------------------------------------------------------------
AddExprNoOp::AddExprNoOp(MulExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void AddExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & AddExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AddExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// AddExprTwoOps
//-----------------------------------------------------------------------------
AddExprTwoOps::AddExprTwoOps(MulExpr & leftExpr, AddExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AddExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AddExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// AddExprAdd
//-----------------------------------------------------------------------------
AddExprAdd::AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AddExprAdd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AddExprSub
//-----------------------------------------------------------------------------
AddExprSub::AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AddExprSub::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AddExprBOr
//-----------------------------------------------------------------------------
AddExprBOr::AddExprBOr(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AddExprBOr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AddExprBXor
//-----------------------------------------------------------------------------
AddExprBXor::AddExprBXor(MulExpr & leftExpr, AddExpr & rightExpr) :
    AddExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AddExprBXor::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
