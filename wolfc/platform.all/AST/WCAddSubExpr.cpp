#include "WCAddSubExpr.hpp"

#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCMulDivExpr.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AddSubExpr
//-----------------------------------------------------------------------------

bool AddSubExpr::peek(const Token * tokenPtr) {
    return MulDivExpr::peek(tokenPtr);
}

AddSubExpr * AddSubExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    MulDivExpr * mulDivExpr = MulDivExpr::parse(tokenPtr, alloc);
    WC_GUARD(mulDivExpr, nullptr);
    
    // See if '+' or '-' following:
    if (tokenPtr->type == TokenType::kPlus) {
        // Add operation: Skip '+'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr, alloc);
        WC_GUARD(addSubExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AddSubExprAdd, *mulDivExpr, *addSubExpr);
    }
    else if (tokenPtr->type == TokenType::kMinus) {
        // Sub operation: Skip '-'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr, alloc);
        WC_GUARD(addSubExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, AddSubExprSub, *mulDivExpr, *addSubExpr);
    }
    
    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, AddSubExprNoOp, *mulDivExpr);
}

//-----------------------------------------------------------------------------
// AddSubExprNoOp
//-----------------------------------------------------------------------------

AddSubExprNoOp::AddSubExprNoOp(MulDivExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & AddSubExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AddSubExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool AddSubExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool AddSubExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & AddSubExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * AddSubExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AddSubExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * AddSubExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// AddSubExprTwoOps
//-----------------------------------------------------------------------------

AddSubExprTwoOps::AddSubExprTwoOps(MulDivExpr & leftExpr, AddSubExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AddSubExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AddSubExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AddSubExprTwoOps::isLValue() {
    return false;
}

bool AddSubExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & AddSubExprTwoOps::dataType() {
    // TODO: handle auto type promotion
    return mLeftExpr.dataType();
}

llvm::Value * AddSubExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    // TODO: would this be true in future for complex types?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't get the address of '+' or '-' operator result!");
    return nullptr;
}

bool AddSubExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// AddSubExprAdd
//-----------------------------------------------------------------------------

AddSubExprAdd::AddSubExprAdd(MulDivExpr & leftExpr, AddSubExpr & rightExpr) :
    AddSubExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddSubExprAdd::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateAdd(left, right, "AddSubExprAdd_AddOp");
}

llvm::Constant * AddSubExprAdd::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getAdd(left, right);
}

//-----------------------------------------------------------------------------
// AddSubExprSub
//-----------------------------------------------------------------------------

AddSubExprSub::AddSubExprSub(MulDivExpr & leftExpr, AddSubExpr & rightExpr) :
    AddSubExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * AddSubExprSub::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSub(left, right, "AddSubExprSub_SubOp");
}

llvm::Constant * AddSubExprSub::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getSub(left, right);
}

WC_END_NAMESPACE
