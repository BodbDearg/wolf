#include "WCNotOrBNotExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCCodegenCtx.hpp"
#include "WCEqExpr.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotOrBNotExpr
//-----------------------------------------------------------------------------

bool NotOrBNotExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNot || EqExpr::peek(tokenPtr);
}

NotOrBNotExpr * NotOrBNotExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Save the first token:
    const Token * startToken = tokenPtr;
    
    // See if an actual 'not' expression:
    if (startToken->type == TokenType::kNot) {
        // Skip 'not'
        ++tokenPtr;
        
        // Parse the not expression following
        NotOrBNotExpr * notExpr = NotOrBNotExpr::parse(tokenPtr, alloc);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(alloc, NotOrBNotExprNot, *notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    EqExpr * addSubExpr = EqExpr::parse(tokenPtr, alloc);
    WC_GUARD(addSubExpr, nullptr);
    return WC_NEW_AST_NODE(alloc, NotOrBNotExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// NotOrBNotExprNoOp
//-----------------------------------------------------------------------------

NotOrBNotExprNoOp::NotOrBNotExprNoOp(EqExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & NotOrBNotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotOrBNotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotOrBNotExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool NotOrBNotExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotOrBNotExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * NotOrBNotExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * NotOrBNotExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * NotOrBNotExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// NotOrBNotExprNot
//-----------------------------------------------------------------------------

NotOrBNotExprNot::NotOrBNotExprNot(NotOrBitNotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & NotOrBNotExprNot::getStartToken() const {
    return mStartToken;
}

const Token & NotOrBNotExprNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotOrBNotExprNot::isLValue() {
    return false;
}

bool NotOrBNotExprNot::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotOrBNotExprNot::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * NotOrBNotExprNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'not' operator result!");
    return nullptr;
}

llvm::Value * NotOrBNotExprNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    compileCheckExprIsBool();

    // Evaluate the expression and generate the code for the not instruction:
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    return cgCtx.irBuilder.CreateNot(value);
}

llvm::Constant * NotOrBNotExprNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    compileCheckExprIsBool();
    
    // Evaluate the expression and generate the code for the not instruction:
    llvm::Constant * value = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(value, nullptr);
    return llvm::ConstantExpr::getNot(value);
}

bool NotOrBNotExprNot::compileCheckExprIsBool() const {
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.isBool()) {
        compileError("Expression following logical 'not' must evaluate to 'bool', not '%s'!",
                     exprType.name().c_str());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
