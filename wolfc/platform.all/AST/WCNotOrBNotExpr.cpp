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
    return  tokenPtr->type == TokenType::kNot ||
            tokenPtr->type == TokenType::kTilde ||
            EqExpr::peek(tokenPtr);
}

NotOrBNotExpr * NotOrBNotExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Save the first token:
    const Token * startToken = tokenPtr;
    
    // See if an actual 'not' or bitwise not ('~') expression:
    if (startToken->type == TokenType::kNot) {
        // Logical 'not': skip 'not'
        ++tokenPtr;
        
        // Parse the expression following
        NotOrBNotExpr * notExpr = NotOrBNotExpr::parse(tokenPtr, alloc);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(alloc, NotOrBNotExprNot, *notExpr, *startToken);
    }
    else if (startToken->type == TokenType::kTilde) {
        // Bitwise 'not': skip 'not'
        ++tokenPtr;

        // Parse the expression following
        NotOrBNotExpr * notExpr = NotOrBNotExpr::parse(tokenPtr, alloc);
        WC_GUARD(notExpr, nullptr);

        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(alloc, NotOrBNotExprBNot, *notExpr, *startToken);
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

NotOrBNotExprNot::NotOrBNotExprNot(NotOrBNotExpr & expr, const Token & startToken) :
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
    WC_GUARD(compileCheckExprIsBool(), nullptr);

    // Evaluate the expression and generate the code for the not instruction:
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    return cgCtx.irBuilder.CreateNot(value);
}

llvm::Constant * NotOrBNotExprNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    WC_GUARD(compileCheckExprIsBool(), nullptr);
    
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

//-----------------------------------------------------------------------------
// NotOrBNotExprBNot
//-----------------------------------------------------------------------------

NotOrBNotExprBNot::NotOrBNotExprBNot(NotOrBNotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & NotOrBNotExprBNot::getStartToken() const {
    return mStartToken;
}

const Token & NotOrBNotExprBNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotOrBNotExprBNot::isLValue() {
    return false;
}

bool NotOrBNotExprBNot::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotOrBNotExprBNot::dataType() {
    return mExpr.dataType();
}

llvm::Value * NotOrBNotExprBNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '~' (bitwise not) operator result!");
    return nullptr;
}

llvm::Value * NotOrBNotExprBNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to an int:
    WC_GUARD(compileCheckExprIsInt(), nullptr);

    // Evaluate the expression and generate the code for the not instruction:
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    return cgCtx.irBuilder.CreateNot(value);
}

llvm::Constant * NotOrBNotExprBNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Expression must evaluate to a boolean:
    WC_GUARD(compileCheckExprIsInt(), nullptr);

    // Evaluate the expression and generate the code for the not instruction:
    llvm::Constant * value = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(value, nullptr);
    return llvm::ConstantExpr::getNot(value);
}

bool NotOrBNotExprBNot::compileCheckExprIsInt() const {
    const DataType & exprType = mExpr.dataType();

    if (!exprType.isInteger()) {
        compileError("Expression following '~' (bitwise not) must evaluate to 'int', not '%s'!",
            exprType.name().c_str());

        return false;
    }

    return true;
}

WC_END_NAMESPACE
