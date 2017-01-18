#include "WCPrimaryExpr.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "WCArrayLit.hpp"
#include "WCBoolLit.hpp"
#include "WCIdentifier.hpp"
#include "WCIntLit.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCRandExpr.hpp"
#include "WCReadnumExpr.hpp"
#include "WCStrLit.hpp"
#include "WCTimeExpr.hpp"

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

void PrimaryExprIntLit::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprIntLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprIntLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprIntLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryExprBoolLit
//-----------------------------------------------------------------------------
PrimaryExprBoolLit::PrimaryExprBoolLit(BoolLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprBoolLit::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprBoolLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprBoolLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprBoolLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryExprStrLit
//-----------------------------------------------------------------------------
PrimaryExprStrLit::PrimaryExprStrLit(StrLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprStrLit::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprStrLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprStrLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprStrLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryExprArrayLit
//-----------------------------------------------------------------------------
PrimaryExprArrayLit::PrimaryExprArrayLit(ArrayLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

void PrimaryExprArrayLit::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprArrayLit::codegenAddrOf(CodegenCtx & cgCtx) {
    return mLit.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprArrayLit::codegenExprEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprArrayLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mLit.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryExprIdentifier
//-----------------------------------------------------------------------------
PrimaryExprIdentifier::PrimaryExprIdentifier(Identifier & ident) : mIdent(ident) {
    mIdent.mParent = this;
}

void PrimaryExprIdentifier::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprIdentifier::codegenAddrOf(CodegenCtx & cgCtx) {
    return mIdent.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprIdentifier::codegenExprEval(CodegenCtx & cgCtx) {
    return mIdent.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprIdentifier::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mIdent.codegenExprConstEval(cgCtx);
}
#endif

const char * PrimaryExprIdentifier::name() const {
    return mIdent.name();
}

//-----------------------------------------------------------------------------
// PrimaryExprReadnum
//-----------------------------------------------------------------------------
PrimaryExprReadnum::PrimaryExprReadnum(ReadnumExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprReadnum::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprReadnum::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprReadnum::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprReadnum::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryExprTime
//-----------------------------------------------------------------------------
PrimaryExprTime::PrimaryExprTime(TimeExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprTime::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprTime::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprTime::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprTime::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PrimaryRandExpr
//-----------------------------------------------------------------------------
PrimaryExprRandExpr::PrimaryExprRandExpr(RandExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PrimaryExprRandExpr::accept(ASTNodeVisitor & visitor) {
    visitor.visit(*this);
}

const Token & PrimaryExprRandExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PrimaryExprRandExpr::getEndToken() const {
    return mExpr.getEndToken();
}

bool PrimaryExprRandExpr::isLValue() {
    return mExpr.isLValue();
}

bool PrimaryExprRandExpr::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & PrimaryExprRandExpr::dataType() {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * PrimaryExprRandExpr::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PrimaryExprRandExpr::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PrimaryExprRandExpr::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
