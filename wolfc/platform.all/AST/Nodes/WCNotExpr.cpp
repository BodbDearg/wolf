#include "WCNotExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCCmpExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotExpr
//-----------------------------------------------------------------------------
bool NotExpr::peek(const Token * tokenPtr) {
    return  tokenPtr->type == TokenType::kNot ||
            tokenPtr->type == TokenType::kTilde ||
            CmpExpr::peek(tokenPtr);
}

NotExpr * NotExpr::parse(ParseCtx & parseCtx) {
    // Save the first token:
    const Token * startToken = parseCtx.curTok;
    
    // See if an actual 'not' or bitwise not ('~') expression:
    if (startToken->type == TokenType::kNot) {
        // Logical 'not': skip 'not'
        parseCtx.nextTok();
        
        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprLNot, *notExpr, *startToken);
    }
    else if (startToken->type == TokenType::kTilde) {
        // Bitwise 'not': skip 'not'
        parseCtx.nextTok();

        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);

        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprBNot, *notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    CmpExpr * addSubExpr = CmpExpr::parse(parseCtx);
    WC_GUARD(addSubExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, NotExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// NotExprNoOp
//-----------------------------------------------------------------------------
NotExprNoOp::NotExprNoOp(CmpExpr & expr) : mExpr(expr) {
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

#warning FIXME - Codegen
#if 0
llvm::Value * NotExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * NotExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * NotExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// NotExprLNot
//-----------------------------------------------------------------------------
NotExprLNot::NotExprLNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & NotExprLNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprLNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprLNot::isLValue() {
    return false;
}

bool NotExprLNot::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotExprLNot::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

#warning FIXME - Codegen
#if 0
llvm::Value * NotExprLNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of 'not' operator result!");
    return nullptr;
}

llvm::Value * NotExprLNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate the expression to be 'notted'
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    
    // Expression must evaluate to a boolean:
    WC_GUARD(compileCheckExprIsBool(), nullptr);
    
    // Create the not operation
    return cgCtx.irBuilder.CreateNot(value, "NotExprNot_NotOp");
}

llvm::Constant * NotExprLNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate the expression to be 'notted'
    llvm::Constant * value = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(value, nullptr);
    
    // Expression must evaluate to a boolean:
    WC_GUARD(compileCheckExprIsBool(), nullptr);
    
    // Create the not operation
    return llvm::ConstantExpr::getNot(value);
}

bool NotExprLNot::compileCheckExprIsBool() const {
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.isBool()) {
        compileError("Expression following logical 'not' must evaluate to 'bool', not '%s'!",
                     exprType.name().c_str());
        
        return false;
    }
    
    return true;
}
#endif

//-----------------------------------------------------------------------------
// NotExprBNot
//-----------------------------------------------------------------------------
NotExprBNot::NotExprBNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

const Token & NotExprBNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprBNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprBNot::isLValue() {
    return false;
}

bool NotExprBNot::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & NotExprBNot::dataType() {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * NotExprBNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '~' (bitwise not) operator result!");
    return nullptr;
}

llvm::Value * NotExprBNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate the expression to be 'notted'
    llvm::Value * val = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(val, nullptr);
    
    // Do the operation and return the result:
    DataType & exprTy = mExpr.dataType();
    return exprTy.codegenBNotOp(cgCtx, *this, *val);
}

llvm::Constant * NotExprBNot::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Evaluate the expression to be 'notted'
    llvm::Constant * val = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(val, nullptr);
    
    // Do the operation and return the result:
    DataType & exprTy = mExpr.dataType();
    return exprTy.codegenConstBNotOp(*this, *val);
}
#endif

#warning FIXME - Codegen
#if 0
bool NotExprBNot::compileCheckExprIsInt() const {
    const DataType & exprType = mExpr.dataType();

    if (!exprType.isInteger()) {
        compileError("Expression following '~' (bitwise not) must evaluate to 'int', not '%s'!",
            exprType.name().c_str());

        return false;
    }

    return true;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
