#include "WCRelExpr.hpp"
#include "WCAddSubExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// RelExpr
//-----------------------------------------------------------------------------

bool RelExpr::peek(const Token * tokenPtr) {
    return AddSubExpr::peek(tokenPtr);
}

RelExpr * RelExpr::parse(const Token *& tokenPtr) {
    AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr);
    WC_GUARD(addSubExpr, nullptr);
    
    // See what tokens follow:
    if (tokenPtr->type == TokenType::kLessThan) {
        ++tokenPtr;     // Skip '<'
        
        if (tokenPtr->type == TokenType::kEquals) {
            ++tokenPtr;                                     // Skip '='
            RelExpr * relExpr = RelExpr::parse(tokenPtr);
            WC_GUARD(relExpr, nullptr);
            return new RelExprLE(*addSubExpr, *relExpr);
        }
        else {
            RelExpr * relExpr = RelExpr::parse(tokenPtr);
            WC_GUARD(relExpr, nullptr);
            return new RelExprLT(*addSubExpr, *relExpr);
        }
    }
    else if (tokenPtr->type == TokenType::kGreaterThan) {
        ++tokenPtr;     // Skip '>'
        
        if (tokenPtr->type == TokenType::kEquals) {
            ++tokenPtr;                                     // Skip '='
            RelExpr * relExpr = RelExpr::parse(tokenPtr);
            WC_GUARD(relExpr, nullptr);
            return new RelExprGE(*addSubExpr, *relExpr);
        }
        else {
            RelExpr * relExpr = RelExpr::parse(tokenPtr);
            WC_GUARD(relExpr, nullptr);
            return new RelExprGT(*addSubExpr, *relExpr);
        }
    }

    // Basic no-op expression:
    return new RelExprNoOp(*addSubExpr);
}

//-----------------------------------------------------------------------------
// RelExprNoOp
//-----------------------------------------------------------------------------

RelExprNoOp::RelExprNoOp(AddSubExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & RelExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & RelExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool RelExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

const DataType & RelExprNoOp::dataType() const {
    return mExpr.dataType();
}

llvm::Value * RelExprNoOp::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * RelExprNoOp::codegenExprEval(const CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

//-----------------------------------------------------------------------------
// RelExprTwoOps
//-----------------------------------------------------------------------------

RelExprTwoOps::RelExprTwoOps(AddSubExpr & leftExpr, RelExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & RelExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & RelExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool RelExprTwoOps::isLValue() const {
    return false;
}

const DataType & RelExprTwoOps::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * RelExprTwoOps::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

bool RelExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// RelExprLT
//-----------------------------------------------------------------------------

RelExprLT::RelExprLT(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprLT::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLT(left, right, "RelExprLT_CmpOp");
}

//-----------------------------------------------------------------------------
// RelExprLE
//-----------------------------------------------------------------------------

RelExprLE::RelExprLE(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprLE::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLE(left, right, "RelExprLE_CmpOp");
}

//-----------------------------------------------------------------------------
// RelExprGT
//-----------------------------------------------------------------------------

RelExprGT::RelExprGT(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprGT::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGT(left, right, "RelExprGT_CmpOp");
}

//-----------------------------------------------------------------------------
// RelExprGE
//-----------------------------------------------------------------------------

RelExprGE::RelExprGE(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprGE::codegenExprEval(const CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    if (!compileCheckBothExprsAreInt()) {
        return nullptr;
    }
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGE(left, right, "RelExprGE_CmpOp");
}

WC_END_NAMESPACE
