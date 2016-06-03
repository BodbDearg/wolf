#include "WCPrimaryExpr.hpp"
#include "WCBoolLit.hpp"
#include "WCIdentifier.hpp"
#include "WCIntLit.hpp"
#include "WCReadnumExpr.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PrimaryExpr
//-----------------------------------------------------------------------------

bool PrimaryExpr::peek(const Token * currentToken) {
    return  IntLit::peek(currentToken) ||
            BoolLit::peek(currentToken) ||
            Identifier::peek(currentToken) ||
            ReadnumExpr::peek(currentToken);
}

PrimaryExpr * PrimaryExpr::parse(const Token *& currentToken) {
    if (IntLit::peek(currentToken)) {
        IntLit * uintLit = IntLit::parse(currentToken);
        WC_GUARD(uintLit, nullptr);
        return new PrimaryExprIntLit(*uintLit);
    }
    else if (BoolLit::peek(currentToken)) {
        BoolLit * boolLit = BoolLit::parse(currentToken);
        WC_GUARD(boolLit, nullptr);
        return new PrimaryExprBoolLit(*boolLit);
    }
    else if (Identifier::peek(currentToken)) {
        Identifier * identifier = Identifier::parse(currentToken);
        WC_GUARD(identifier, nullptr);
        return new PrimaryExprIdentifier(*identifier);
    }
    else if (ReadnumExpr::peek(currentToken)) {
        ReadnumExpr * expr = ReadnumExpr::parse(currentToken);
        WC_GUARD(expr, nullptr);
        return new PrimaryExprReadnum(*expr);
    }
    
    parseError(*currentToken, "Expected primary expression!");
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprIntLit
//-----------------------------------------------------------------------------

PrimaryExprIntLit::PrimaryExprIntLit(IntLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

const Token & PrimaryExprIntLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprIntLit::getEndToken() const {
    return mLit.getEndToken();
}

llvm::Value * PrimaryExprIntLit::generateCode(const CodegenCtx & cgCtx) {
    return mLit.generateCode(cgCtx);
}

bool PrimaryExprIntLit::isLValue() const {
    return false;
}

llvm::Value * PrimaryExprIntLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    compileError("Can't take the address of an int literal expression!");
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprBoolLit
//-----------------------------------------------------------------------------

PrimaryExprBoolLit::PrimaryExprBoolLit(BoolLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

const Token & PrimaryExprBoolLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprBoolLit::getEndToken() const {
    return mLit.getEndToken();
}

llvm::Value * PrimaryExprBoolLit::generateCode(const CodegenCtx & cgCtx) {
    return mLit.generateCode(cgCtx);
}

bool PrimaryExprBoolLit::isLValue() const {
    return false;
}

llvm::Value * PrimaryExprBoolLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    compileError("Can't take the address of a bool literal expression!");
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprIdentifier
//-----------------------------------------------------------------------------

PrimaryExprIdentifier::PrimaryExprIdentifier(Identifier & identifier) : mIdentifier(identifier) {
    mIdentifier.mParent = this;
}

const Token & PrimaryExprIdentifier::getStartToken() const {
    return mIdentifier.getStartToken();
}

const Token & PrimaryExprIdentifier::getEndToken() const {
    return mIdentifier.getEndToken();
}

llvm::Value * PrimaryExprIdentifier::generateCode(const CodegenCtx & cgCtx) {
    return mIdentifier.generateCode(cgCtx);
}

bool PrimaryExprIdentifier::isLValue() const {
    return true;
}

llvm::Value * PrimaryExprIdentifier::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mIdentifier.codegenAddrOf(cgCtx);
}

//-----------------------------------------------------------------------------
// PrimaryExprReadnum
//-----------------------------------------------------------------------------

PrimaryExprReadnum::PrimaryExprReadnum(ReadnumExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}
    
const Token & PrimaryExprReadnum::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprReadnum::getEndToken() const {
    return mExpr.getEndToken();
}
    
llvm::Value * PrimaryExprReadnum::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}
    
bool PrimaryExprReadnum::isLValue() const {
    return false;
}
    
llvm::Value * PrimaryExprReadnum::codegenAddrOf(const CodegenCtx & cgCtx) {
    compileError("Can't take the address of a readnum expression!");
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

WC_END_NAMESPACE
