//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "PrimaryExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "ArrayLit.hpp"
#include "BoolLit.hpp"
#include "Identifier.hpp"
#include "IntLit.hpp"
#include "LinearAlloc.hpp"
#include "NullLit.hpp"
#include "RandExpr.hpp"
#include "ReadnumExpr.hpp"
#include "StrLit.hpp"
#include "TimeExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PrimaryExpr
//-----------------------------------------------------------------------------
bool PrimaryExpr::peek(const Token * currentToken) {
    return  IntLit::peek(currentToken) ||
            BoolLit::peek(currentToken) ||
            StrLit::peek(currentToken) ||
            NullLit::peek(currentToken) ||
            ArrayLit::peek(currentToken) ||
            Identifier::peek(currentToken) ||
            ReadnumExpr::peek(currentToken) ||
            TimeExpr::peek(currentToken) ||
            RandExpr::peek(currentToken);
}

PrimaryExpr * PrimaryExpr::parse(ParseCtx & parseCtx) {
    /* Useful macro to eliminate a lot of repetition */
    #define TRY_PARSE_PRIMARY_EXPR_TYPE(ExprType)\
        if (ExprType::peek(parseCtx.tok())) {\
            ExprType * expr = ExprType::parse(parseCtx);\
            WC_GUARD(expr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, PrimaryExpr##ExprType, *expr);\
        }
    
    TRY_PARSE_PRIMARY_EXPR_TYPE(IntLit)
    TRY_PARSE_PRIMARY_EXPR_TYPE(BoolLit)
    TRY_PARSE_PRIMARY_EXPR_TYPE(StrLit)
    TRY_PARSE_PRIMARY_EXPR_TYPE(NullLit)
    TRY_PARSE_PRIMARY_EXPR_TYPE(ArrayLit)
    TRY_PARSE_PRIMARY_EXPR_TYPE(Identifier)
    TRY_PARSE_PRIMARY_EXPR_TYPE(ReadnumExpr)
    TRY_PARSE_PRIMARY_EXPR_TYPE(TimeExpr)
    TRY_PARSE_PRIMARY_EXPR_TYPE(RandExpr)

    #undef TRY_PARSE_PRIMARY_EXPR_TYPE
    
    parseCtx.error("Expected primary expression!");
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprIntLit
//-----------------------------------------------------------------------------
PrimaryExprIntLit::PrimaryExprIntLit(IntLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprIntLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprIntLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprIntLit::getEndToken() const {
    return mLit.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprBoolLit
//-----------------------------------------------------------------------------
PrimaryExprBoolLit::PrimaryExprBoolLit(BoolLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprBoolLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprBoolLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprBoolLit::getEndToken() const {
    return mLit.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprStrLit
//-----------------------------------------------------------------------------
PrimaryExprStrLit::PrimaryExprStrLit(StrLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprStrLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprStrLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprStrLit::getEndToken() const {
    return mLit.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprNullLit
//-----------------------------------------------------------------------------
PrimaryExprNullLit::PrimaryExprNullLit(NullLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprNullLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprNullLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprNullLit::getEndToken() const {
    return mLit.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprArrayLit
//-----------------------------------------------------------------------------
PrimaryExprArrayLit::PrimaryExprArrayLit(ArrayLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprArrayLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprArrayLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprArrayLit::getEndToken() const {
    return mLit.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprIdentifier
//-----------------------------------------------------------------------------
PrimaryExprIdentifier::PrimaryExprIdentifier(Identifier & ident) : mIdent(ident) {
    mIdent.mParent = this;
}

void PrimaryExprIdentifier::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprIdentifier::getStartToken() const {
    return mIdent.getStartToken();
}

const Token & PrimaryExprIdentifier::getEndToken() const {
    return mIdent.getEndToken();
}

const char * PrimaryExprIdentifier::name() const {
    return mIdent.name();
}

//-----------------------------------------------------------------------------
// PrimaryExprReadnumExpr
//-----------------------------------------------------------------------------
PrimaryExprReadnumExpr::PrimaryExprReadnumExpr(ReadnumExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprReadnumExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & PrimaryExprReadnumExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprReadnumExpr::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryExprTimeExpr
//-----------------------------------------------------------------------------
PrimaryExprTimeExpr::PrimaryExprTimeExpr(TimeExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprTimeExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprTimeExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprTimeExpr::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// PrimaryRandExpr
//-----------------------------------------------------------------------------
PrimaryExprRandExpr::PrimaryExprRandExpr(RandExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprRandExpr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprRandExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprRandExpr::getEndToken() const {
    return mExpr.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
