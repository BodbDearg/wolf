#include "WCNotExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCCodegenCtx.hpp"
#include "WCEqExpr.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotExpr
//-----------------------------------------------------------------------------

bool NotExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNot || EqExpr::peek(tokenPtr);
}

NotExpr * NotExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Save the first token:
    const Token * startToken = tokenPtr;
    
    // See if an actual 'not' expression:
    if (startToken->type == TokenType::kNot) {
        // Skip 'not'
        ++tokenPtr;
        
        // Parse the not expression following
        NotExpr * notExpr = NotExpr::parse(tokenPtr, alloc);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(alloc, NotExprNot, *notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    EqExpr * addSubExpr = EqExpr::parse(tokenPtr, alloc);
    WC_GUARD(addSubExpr, nullptr);
    return WC_NEW_AST_NODE(alloc, NotExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// NotExprNoOp
//-----------------------------------------------------------------------------

NotExprNoOp::NotExprNoOp(EqExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & NotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool NotExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * NotExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * NotExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * NotExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// NotExprNot
//-----------------------------------------------------------------------------

NotExprNot::NotExprNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & NotExprNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprNot::isLValue() {
    return false;
}

bool NotExprNot::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotExprNot::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * NotExprNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'not' operator result!");
    return nullptr;
}

llvm::Value * NotExprNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    compileCheckExprIsBool();

    // Evaluate the expression and generate the code for the not instruction:
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    return cgCtx.irBuilder.CreateNot(value);
}

llvm::Constant * NotExprNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    compileCheckExprIsBool();
    
    // Evaluate the expression and generate the code for the not instruction:
    llvm::Constant * value = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(value, nullptr);
    return llvm::ConstantExpr::getNot(value);
}

bool NotExprNot::compileCheckExprIsBool() const {
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.isBool()) {
        compileError("Expression following logical 'not' must evaluate to 'bool', not '%s'!",
                     exprType.name().c_str());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
