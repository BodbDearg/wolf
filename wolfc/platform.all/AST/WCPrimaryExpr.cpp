#include "WCPrimaryExpr.hpp"

#include "Lexer/WCToken.hpp"
#include "WCArrayLit.hpp"
#include "WCBoolLit.hpp"
#include "WCIdentifier.hpp"
#include "WCIntLit.hpp"
#include "WCLinearAlloc.hpp"
#include "WCReadnumExpr.hpp"
#include "WCStrLit.hpp"
#include "WCTimeExpr.hpp"

WC_BEGIN_NAMESPACE

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
            TimeExpr::peek(currentToken);
}

PrimaryExpr * PrimaryExpr::parse(const Token *& currentToken, LinearAlloc & alloc) {
    if (IntLit::peek(currentToken)) {
        IntLit * uintLit = IntLit::parse(currentToken, alloc);
        WC_GUARD(uintLit, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprIntLit, *uintLit);
    }
    else if (BoolLit::peek(currentToken)) {
        BoolLit * boolLit = BoolLit::parse(currentToken, alloc);
        WC_GUARD(boolLit, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprBoolLit, *boolLit);
    }
    else if (StrLit::peek(currentToken)) {
        StrLit * strLit = StrLit::parse(currentToken, alloc);
        WC_GUARD(strLit, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprStrLit, *strLit);
    }
    else if (ArrayLit::peek(currentToken)) {
        ArrayLit * arrayLit = ArrayLit::parse(currentToken, alloc);
        WC_GUARD(arrayLit, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprArrayLit, *arrayLit);
    }
    else if (Identifier::peek(currentToken)) {
        Identifier * identifier = Identifier::parse(currentToken, alloc);
        WC_GUARD(identifier, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprIdentifier, *identifier);
    }
    else if (ReadnumExpr::peek(currentToken)) {
        ReadnumExpr * expr = ReadnumExpr::parse(currentToken, alloc);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprReadnum, *expr);
    }
    else if (TimeExpr::peek(currentToken)) {
        TimeExpr * expr = TimeExpr::parse(currentToken, alloc);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(alloc, PrimaryExprTime, *expr);
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

bool PrimaryExprIntLit::isLValue() {
    return mLit.isLValue();
}

bool PrimaryExprIntLit::isConstExpr() {
    return mLit.isConstExpr();
}

DataType & PrimaryExprIntLit::dataType() {
    return mLit.dataType();
}

llvm::Value * PrimaryExprIntLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprIntLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprIntLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
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

bool PrimaryExprBoolLit::isLValue() {
    return mLit.isLValue();
}

bool PrimaryExprBoolLit::isConstExpr() {
    return mLit.isConstExpr();
}

DataType & PrimaryExprBoolLit::dataType() {
    return mLit.dataType();
}

llvm::Value * PrimaryExprBoolLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprBoolLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprBoolLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
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

bool PrimaryExprStrLit::isLValue() {
    return mLit.isLValue();
}

bool PrimaryExprStrLit::isConstExpr() {
    return mLit.isConstExpr();
}

DataType & PrimaryExprStrLit::dataType() {
    return mLit.dataType();
}

llvm::Value * PrimaryExprStrLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprStrLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprStrLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// PrimaryExprArrayLit
//-----------------------------------------------------------------------------

PrimaryExprArrayLit::PrimaryExprArrayLit(ArrayLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

const Token & PrimaryExprArrayLit::getStartToken() const {
    return mLit.getStartToken();
}

const Token & PrimaryExprArrayLit::getEndToken() const {
    return mLit.getEndToken();
}

bool PrimaryExprArrayLit::isLValue() {
    return mLit.isLValue();
}

bool PrimaryExprArrayLit::isConstExpr() {
    return mLit.isConstExpr();
}

DataType & PrimaryExprArrayLit::dataType() {
    return mLit.dataType();
}

llvm::Value * PrimaryExprArrayLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprArrayLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprArrayLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
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

bool PrimaryExprIdentifier::isLValue() {
    return mIdent.isLValue();
}

bool PrimaryExprIdentifier::isConstExpr() {
    return mIdent.isConstExpr();
}

DataType & PrimaryExprIdentifier::dataType() {
    return mIdent.dataType();
}

llvm::Value * PrimaryExprIdentifier::codegenAddrOf(CodegenCtx & cgCtx) {
    return mIdent.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprIdentifier::codegenExprEval(CodegenCtx & cgCtx) {
    return mIdent.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprIdentifier::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mIdent.codegenExprConstEval(cgCtx);
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
    
const Token & PrimaryExprReadnum::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprReadnum::getEndToken() const {
    return mExpr.getEndToken();
}
    
bool PrimaryExprReadnum::isLValue() {
    return mExpr.isLValue();
}

bool PrimaryExprReadnum::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & PrimaryExprReadnum::dataType() {
    return mExpr.dataType();
}

llvm::Value * PrimaryExprReadnum::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprReadnum::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprReadnum::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// PrimaryExprTime
//-----------------------------------------------------------------------------

PrimaryExprTime::PrimaryExprTime(TimeExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & PrimaryExprTime::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprTime::getEndToken() const {
    return mExpr.getEndToken();
}

bool PrimaryExprTime::isLValue() {
    return mExpr.isLValue();
}

bool PrimaryExprTime::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & PrimaryExprTime::dataType() {
    return mExpr.dataType();
}

llvm::Value * PrimaryExprTime::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprTime::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprTime::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

WC_END_NAMESPACE
