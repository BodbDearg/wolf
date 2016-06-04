#include "WCPrimaryExpr.hpp"
#include "WCBoolLit.hpp"
#include "WCIdentifier.hpp"
#include "WCIntLit.hpp"
#include "WCReadnumExpr.hpp"
#include "WCStrLit.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PrimaryExpr
//-----------------------------------------------------------------------------

bool PrimaryExpr::peek(const Token * currentToken) {
    return  IntLit::peek(currentToken) ||
            BoolLit::peek(currentToken) ||
            StrLit::peek(currentToken) ||
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
    else if (StrLit::peek(currentToken)) {
        StrLit * strLit = StrLit::parse(currentToken);
        WC_GUARD(strLit, nullptr);
        return new PrimaryExprStrLit(*strLit);
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

const DataType & PrimaryExprIntLit::getDataType() const {
    return mLit.getDataType();
}

llvm::Value * PrimaryExprIntLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
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

const DataType & PrimaryExprBoolLit::getDataType() const {
    return mLit.getDataType();
}

llvm::Value * PrimaryExprBoolLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprStrLit
//-----------------------------------------------------------------------------

PrimaryExprStrLit::PrimaryExprStrLit(StrLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

const Token & PrimaryExprStrLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprStrLit::getEndToken() const {
    return mLit.getEndToken();
}

llvm::Value * PrimaryExprStrLit::generateCode(const CodegenCtx & cgCtx) {
    return mLit.generateCode(cgCtx);
}

bool PrimaryExprStrLit::isLValue() const {
    return false;
}

const DataType & PrimaryExprStrLit::getDataType() const {
    return mLit.getDataType();
}

llvm::Value * PrimaryExprStrLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

//-----------------------------------------------------------------------------
// PrimaryExprIdentifier
//-----------------------------------------------------------------------------

PrimaryExprIdentifier::PrimaryExprIdentifier(Identifier & ident) : mIdent(ident) {
    mIdent.mParent = this;
}

const Token & PrimaryExprIdentifier::getStartToken() const {
    return mIdent.getStartToken();
}

const Token & PrimaryExprIdentifier::getEndToken() const {
    return mIdent.getEndToken();
}

llvm::Value * PrimaryExprIdentifier::generateCode(const CodegenCtx & cgCtx) {
    return mIdent.generateCode(cgCtx);
}

bool PrimaryExprIdentifier::isLValue() const {
    return true;
}

const DataType & PrimaryExprIdentifier::getDataType() const {
    return mIdent.getDataType();
}

llvm::Value * PrimaryExprIdentifier::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mIdent.codegenAddrOf(cgCtx);
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

const DataType & PrimaryExprReadnum::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * PrimaryExprReadnum::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

WC_END_NAMESPACE
