#include "WCAddSubExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
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

AddSubExpr * AddSubExpr::parse(const Token *& tokenPtr) {
    MulDivExpr * mulDivExpr = MulDivExpr::parse(tokenPtr);
    WC_GUARD(mulDivExpr, nullptr);
    
    // See if '+' or '-' following:
    if (tokenPtr->type == TokenType::kPlus) {
        // Add operation: Skip '+'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr);
        return new AddSubExprAdd(*mulDivExpr, *addSubExpr);
    }
    else if (tokenPtr->type == TokenType::kAsterisk) {
        // Sub operation: Skip '-'
        ++tokenPtr;
        
        // Parse following expr and return combined expr
        AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr);
        return new AddSubExprSub(*mulDivExpr, *addSubExpr);
    }
    
    // Basic no-op expression:
    return new AddSubExprNoOp(*mulDivExpr);
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

bool AddSubExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & AddSubExprNoOp::getDataType() const {
    return mExpr.getDataType();
}

llvm::Value * AddSubExprNoOp::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * AddSubExprNoOp::generateCode(const CodegenCtx & cgCtx) {
    return mExpr.generateCode(cgCtx);
}

//-----------------------------------------------------------------------------
// AddSubExprAdd
//-----------------------------------------------------------------------------

AddSubExprAdd::AddSubExprAdd(MulDivExpr & leftExpr, AddSubExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AddSubExprAdd::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AddSubExprAdd::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AddSubExprAdd::isLValue() const {
    return false;
}

const DataType & AddSubExprAdd::getDataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.getDataType();
}

bool AddSubExprAdd::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.getDataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.getDataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

llvm::Value * AddSubExprAdd::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * AddSubExprAdd::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateAdd(left, right, "AddSubExprAdd_AddOp");
}

//-----------------------------------------------------------------------------
// AddSubExprSub
//-----------------------------------------------------------------------------

AddSubExprSub::AddSubExprSub(MulDivExpr & leftExpr, AddSubExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AddSubExprSub::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AddSubExprSub::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AddSubExprSub::isLValue() const {
    return false;
}

const DataType & AddSubExprSub::getDataType() const {
    // TODO: handle auto type promotion
    return mLeftExpr.getDataType();
}

bool AddSubExprSub::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.getDataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.getDataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

llvm::Value * AddSubExprSub::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * AddSubExprSub::generateCode(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.generateCode(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.generateCode(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateSub(left, right, "AddSubExprSub_SubOp");
}

WC_END_NAMESPACE
