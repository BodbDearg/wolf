#include "WCNotExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCEqExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

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
    expr.mParent = this;
}

const Token & NotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & NotExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * NotExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * NotExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
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

bool NotExprNot::isLValue() const {
    return false;
}

const DataType & NotExprNot::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * NotExprNot::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * NotExprNot::codegenExprEval(CodegenCtx & cgCtx) {
    // Grab the type for bool
    const DataType & boolType = PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
    
    // Expression must evaluate to a boolean:
    const DataType & exprType = mExpr.dataType();
    
    if (!exprType.equals(boolType)) {
        compileError("Expression following logical 'not' must evaluate to 'bool', not '%s'!", exprType.name());
        return nullptr;
    }

    // Evaluate the expression:
    llvm::Value * value = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(value, nullptr);
    
    // Generate and return bitwise not instruction:
    return cgCtx.irBuilder.CreateNot(value);
}

WC_END_NAMESPACE
