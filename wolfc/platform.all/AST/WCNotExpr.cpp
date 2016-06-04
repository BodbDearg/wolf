#include "WCNotExpr.hpp"
#include "WCAddSubExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotExpr
//-----------------------------------------------------------------------------

bool NotExpr::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kNot || AddSubExpr::peek(tokenPtr);
}

NotExpr * NotExpr::parse(const Token *& tokenPtr) {
    // Save the first token:
    const Token * startToken = tokenPtr;
    
    // See if an actual 'not' expression:
    if (startToken->type == TokenType::kNot) {
        // Skip 'not'
        ++tokenPtr;
        
        // Parse the not expression following
        NotExpr * notExpr = NotExpr::parse(tokenPtr);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return new NotExprNot(*notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr);
    WC_GUARD(addSubExpr, nullptr);
    return new NotExprNoOp(*addSubExpr);
}

//-----------------------------------------------------------------------------
// NotExprNoOp
//-----------------------------------------------------------------------------

NotExprNoOp::NotExprNoOp(AddSubExpr & expr) : mExpr(expr) {
    expr.mParent = this;
}

const Token & NotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * NotExprNoOp::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

bool NotExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & NotExprNoOp::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * NotExprNoOp::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
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

llvm::Value * NotExprNot::generateCode(const CodegenCtx & cgCtx) {
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Expression must evaluate to a boolean:
    const DataType & exprType = mExpr.getDataType();
    
    if (!exprType.equals(boolType)) {
        compileError("Expression following logical 'not' must evaluate to 'bool', not '%s'!", exprType.name());
        return nullptr;
    }

    // Evaluate the expression:
    llvm::Value * value = mExpr.generateCode(cgCtx);
    WC_GUARD(value, nullptr);
    
    // Generate and return bitwise not instruction:
    return cgCtx.irBuilder.CreateNot(value);
}

bool NotExprNot::isLValue() const {
    return false;
}

const DataType & NotExprNot::getDataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * NotExprNot::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

WC_END_NAMESPACE
