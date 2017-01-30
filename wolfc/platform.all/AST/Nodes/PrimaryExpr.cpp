#include "PrimaryExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "ArrayLit.hpp"
#include "BoolLit.hpp"
#include "Identifier.hpp"
#include "IntLit.hpp"
#include "LinearAlloc.hpp"
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
            ArrayLit::peek(currentToken) ||
            Identifier::peek(currentToken) ||
            ReadnumExpr::peek(currentToken) ||
            TimeExpr::peek(currentToken) ||
            RandExpr::peek(currentToken);
}

PrimaryExpr * PrimaryExpr::parse(ParseCtx & parseCtx) {
    if (IntLit::peek(parseCtx.tok())) {
        IntLit * uintLit = IntLit::parse(parseCtx);
        WC_GUARD(uintLit, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprIntLit, *uintLit);
    }
    else if (BoolLit::peek(parseCtx.tok())) {
        BoolLit * boolLit = BoolLit::parse(parseCtx);
        WC_GUARD(boolLit, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprBoolLit, *boolLit);
    }
    else if (StrLit::peek(parseCtx.tok())) {
        StrLit * strLit = StrLit::parse(parseCtx);
        WC_GUARD(strLit, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprStrLit, *strLit);
    }
    else if (ArrayLit::peek(parseCtx.tok())) {
        ArrayLit * arrayLit = ArrayLit::parse(parseCtx);
        WC_GUARD(arrayLit, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprArrayLit, *arrayLit);
    }
    else if (Identifier::peek(parseCtx.tok())) {
        Identifier * identifier = Identifier::parse(parseCtx);
        WC_GUARD(identifier, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprIdentifier, *identifier);
    }
    else if (ReadnumExpr::peek(parseCtx.tok())) {
        ReadnumExpr * expr = ReadnumExpr::parse(parseCtx);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprReadnum, *expr);
    }
    else if (TimeExpr::peek(parseCtx.tok())) {
        TimeExpr * expr = TimeExpr::parse(parseCtx);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprTime, *expr);
    }
    else if (RandExpr::peek(parseCtx.tok())) {
        RandExpr * expr = RandExpr::parse(parseCtx);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(parseCtx, PrimaryExprRandExpr, *expr);
    }
    
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

bool PrimaryExprIntLit::isLValue() const {
    return mLit.isLValue();
}

bool PrimaryExprIntLit::isConstExpr() const {
    return mLit.isConstExpr();
}

const DataType & PrimaryExprIntLit::dataType() const {
    return mLit.dataType();
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

bool PrimaryExprBoolLit::isLValue() const {
    return mLit.isLValue();
}

bool PrimaryExprBoolLit::isConstExpr() const {
    return mLit.isConstExpr();
}

const DataType & PrimaryExprBoolLit::dataType() const {
    return mLit.dataType();
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

bool PrimaryExprStrLit::isLValue() const {
    return mLit.isLValue();
}

bool PrimaryExprStrLit::isConstExpr() const {
    return mLit.isConstExpr();
}

const DataType & PrimaryExprStrLit::dataType() const {
    return mLit.dataType();
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

bool PrimaryExprArrayLit::isLValue() const {
    return mLit.isLValue();
}

bool PrimaryExprArrayLit::isConstExpr() const {
    return mLit.isConstExpr();
}

const DataType & PrimaryExprArrayLit::dataType() const {
    return mLit.dataType();
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

bool PrimaryExprIdentifier::isLValue() const {
    return mIdent.isLValue();
}

bool PrimaryExprIdentifier::isConstExpr() const {
    return mIdent.isConstExpr();
}

const DataType & PrimaryExprIdentifier::dataType() const {
    return mIdent.dataType();
}

const char * PrimaryExprIdentifier::name() const {
    return mIdent.name();
}

//-----------------------------------------------------------------------------
// PrimaryExprReadnum
//-----------------------------------------------------------------------------
PrimaryExprReadnum::PrimaryExprReadnum(ReadnumExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprReadnum::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}
    
const Token & PrimaryExprReadnum::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprReadnum::getEndToken() const {
    return mExpr.getEndToken();
}
    
bool PrimaryExprReadnum::isLValue() const {
    return mExpr.isLValue();
}

bool PrimaryExprReadnum::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & PrimaryExprReadnum::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// PrimaryExprTime
//-----------------------------------------------------------------------------
PrimaryExprTime::PrimaryExprTime(TimeExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprTime::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrimaryExprTime::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprTime::getEndToken() const {
    return mExpr.getEndToken();
}

bool PrimaryExprTime::isLValue() const {
    return mExpr.isLValue();
}

bool PrimaryExprTime::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & PrimaryExprTime::dataType() const {
    return mExpr.dataType();
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

bool PrimaryExprRandExpr::isLValue() const {
    return mExpr.isLValue();
}

bool PrimaryExprRandExpr::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & PrimaryExprRandExpr::dataType() const {
    return mExpr.dataType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
